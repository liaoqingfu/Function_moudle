
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string>
using namespace std;

#if 0
char* html = NULL;
int len = 0;
#endif



size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    printf("once\n");
    // [first, last)
    string& data = *(string*)userdata;
    copy(ptr, ptr+size*nmemb, back_inserter(data));

#if 0
    if(html == NULL)
    {
        html = (char*)malloc(size*nmemb+1);
    }
    else
    {
        html = (char*)realloc(html, len + size* nmemb+1);
    }
    memcpy(html+len, ptr, size*nmemb);
    len += size * nmemb;
    html[len] = 0;
#endif
    return size* nmemb;
}

int main()
{
    CURL* curl = curl_easy_init();

    string data;

    curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    printf("start perform\n");
    CURLcode code = curl_easy_perform(curl);
    printf ("end perform\n");
    if(code != CURLE_OK)
    {
        printf("curl_easy_perform error = %d\n", (int)code);
        return 0;
    }

   // printf("html is:\n%s\n", data.c_str());

    curl_easy_cleanup(curl);
}
