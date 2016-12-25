#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#include <curl/curl.h>
#pragma clang diagnostic pop
#include <minizip/unzip.h>

#include "run_vault.h"
#include "vault_version.h"

#define EXTRACT_BUFFERSIZE 8192


static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

void download_vault() {
    if (access(VAULT_DOWNLOAD_ARCHIVE, F_OK) != 0) {
        // Download
        printf("Did not find vault locally, downloading\n");
        FILE* vault_archive = fopen(VAULT_DOWNLOAD_ARCHIVE, "wb");
        if (vault_archive) {
            curl_global_init(CURL_GLOBAL_DEFAULT);
            CURL* curl = curl_easy_init();
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
            curl_easy_setopt(curl, CURLOPT_URL, VAULT_DOWNLOAD_URL);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, vault_archive);
#pragma clang diagnostic pop
            curl_easy_perform(curl);
            fclose(vault_archive);
            curl_easy_cleanup(curl);
        }
    } else {
        printf("%s already exists, skipping download.\n", VAULT_DOWNLOAD_ARCHIVE);
    }
}

void unpack_vault() {
    const char* vault_filename = "vault";

    if (access(VAULT_DOWNLOAD_ARCHIVE, F_OK) == 0 &&
        access(VAULT_DOWNLOAD_EXECUTABLE, F_OK) != 0) {
        printf("Found %s unpacking\n", VAULT_DOWNLOAD_ARCHIVE);
        unzFile archive = unzOpen(VAULT_DOWNLOAD_ARCHIVE);
        printf("Archive %p\n", archive);
        if (unzLocateFile(archive, vault_filename, 0) != UNZ_OK) {
            printf("Did not find '%s' in downloaded archive\n",vault_filename);
            exit(2);
        }
        FILE* vault_executable_f = fopen(VAULT_DOWNLOAD_EXECUTABLE, "wb");
        if (!vault_executable_f) {
            printf("Failed to open %s, for writing\n", VAULT_DOWNLOAD_EXECUTABLE);
            exit(2);
        }

        unzOpenCurrentFile(archive);
        int unzip_err = 0;
        void* extract_buffer = malloc(EXTRACT_BUFFERSIZE);
        do {
            unzip_err = unzReadCurrentFile(archive, extract_buffer, EXTRACT_BUFFERSIZE);
            if (unzip_err < 0)
            {
                printf("error %d with archive in unzReadCurrentFile\n", unzip_err);
                exit(2);
            }
            if (unzip_err > 0) {
                // If unzip_err > 0 then unzip_err = number of bytes read
                if (fwrite(extract_buffer, (size_t)unzip_err, 1, vault_executable_f) == 0) {
                    printf("Failed to write to extracted file\n");
                    exit(2);
                }
            }
        } while (unzip_err != 0);
        fclose(vault_executable_f);
        unzCloseCurrentFile(archive);
        free(extract_buffer);

        // Make the vault executalbe executable
        chmod(VAULT_DOWNLOAD_EXECUTABLE, S_IRWXU);
    };
}

static FILE* vault_process;
static const char server_args[] = "server -dev -dev-root-token-id=\"vault_testing\"";

void run_vault() {
    printf("Starting vault");
    char* command = malloc(sizeof(VAULT_DOWNLOAD_EXECUTABLE) + sizeof(server_args));
    sprintf(command, "%s %s", VAULT_DOWNLOAD_EXECUTABLE, server_args);
    vault_process = popen(command, "r");
    free(command);
    sleep(1);
}

void stop_vault() {
    if (vault_process != 0) {
        pclose(vault_process);
    }
}
