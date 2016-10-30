#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h>

#include <curl/curl.h>
#include <minizip/unzip.h>

#include "vault_version.h"

void download_vault();

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

void download_vault() {
    if (access(VAULT_DOWNLOAD_ARCHIVE, F_OK) != 0) {
        // Download
        printf("Did not find vault locally, downloading\n");
        curl_global_init(CURL_GLOBAL_DEFAULT);
        CURL* curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, VAULT_DOWNLOAD_URL); 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
        FILE* vault_archive = fopen(VAULT_DOWNLOAD_ARCHIVE, "wb");
        if (vault_archive) {
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, vault_archive);
            curl_easy_perform(curl);
            fclose(vault_archive);
        }
        curl_easy_cleanup(curl);
    }
}

void unpack_vault();
void unpack_vault() {
    
};

int main() {
    download_vault();
    unpack_vault();
    return 0;

    pid_t vault_pid = fork();

    if (vault_pid == 0) {
        static char* args[] = {"vault", "server", "-dev", NULL};
        execv("/usr/home/tbartelmess/libvault/test/vault", args);
        printf("Vault's fault\n");
        exit(99);
    }
        for (int i=0; i<5; i++) {
            sleep(1);
            printf("Sleeping %d\n", i);
        }
        printf("Time to kill vault\n");
        kill(vault_pid, SIGINT);
        waitpid(vault_pid, 0, 0);
        printf("Vault killed\n");
    return 0;
}
