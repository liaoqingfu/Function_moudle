
#include <curl/curl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cJSON.h"

/*
    {
        result: ok,
    }

    or

    {
        result: err,
        reason: xxxxx
    }
*/
#if 0
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    // handle server response
    return size*nmemb;

    // json
}
#endif

/*
<note>
<to>George</to>
<from>John</from>
<heading>Reminder</heading>
<body>Don't forget the meeting!</body>
</note>
*/
/*
{
    "node": {
        "to": "George",
        "from": "John",
        "heading":"Reminder"
        "body":"Don't forget the meeting!"
    }
}
*/

char* get_json(const char* username, const char* password)
{
    cJSON* root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "username", username);
    cJSON_AddStringToObject(root, "password", password);

    char* ret = cJSON_Print(root);
    cJSON_Delete(root);

    return ret;
}


int main()
{
    // easy是阻塞方式的
    CURL* curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1/cgi-bin/a.out");

    char* data = get_json("aaa", "bbbsdfsdf");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

//    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    curl_easy_perform(curl);  // request --> server
                            // server --> response_data1  write_callback
                            // server --> response_data2  write_callback
                            // server --> response_data3  write_callback

    free(data);

    curl_easy_cleanup(curl);
}
