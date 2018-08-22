
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include "cJSON.h"

#if 0
{
    "key1": 1,
    "key2": true,
    "key3": false,
    "key4": none,
    "key5": { "key51": "hello", "key52":"world" },
    "key6": [
            {"key611": true},
            {"key621": false},
            {"key631":true},
            40
        ]
}

value: none, bool, number, string, object, array
#endif

char* get_json()
{
    cJSON* root = cJSON_CreateObject();
    // root = {}
    cJSON_AddTrueToObject(root, "key2");
    cJSON_AddNumberToObject(root, "key1", 1);
    cJSON_AddFalseToObject(root, "key3");

    cJSON_AddNullToObject(root, "key4");
   // cJSON_AddItemToObject(root, "key4", cJSON_CreateNull());

    cJSON* key5 = cJSON_CreateObject();
    cJSON_AddStringToObject(key5, "key51", "hello");
//    cJSON_AddItemToObject(key5, "key51", cJSON_CreateString("hello"));
    cJSON_AddStringToObject(key5, "key52", "world");
    cJSON_AddItemToObject(root, "key5", key5);

    cJSON* key6 = cJSON_CreateArray();

    cJSON* key61 = cJSON_CreateObject();
    cJSON_AddTrueToObject(key61, "key611");
    cJSON_AddItemToArray(key6, key61);

    cJSON* key62 = cJSON_CreateObject();
    cJSON_AddFalseToObject(key62, "key621");
    cJSON_AddItemToArray(key6, key62);

    cJSON* key63 = cJSON_CreateObject();
    cJSON_AddTrueToObject(key63, "key631");
    cJSON_AddItemToArray(key6, key63);

    cJSON_AddItemToArray(key6, cJSON_CreateNumber(40));

    cJSON_AddItemToObject(root, "key6", key6);

    char* p = cJSON_Print(root);
    cJSON_Delete(root);
    return p;
}

int main()
{
    char* p = get_json();

    cJSON* root = cJSON_Parse(p);

    cJSON* key6 = cJSON_GetObjectItem(root, "key6");
    if(key6->type == cJSON_Array)
    {
        printf("key6 is array\n");
    }

    cJSON* key62 = cJSON_GetArrayItem(key6, 1);
    cJSON* key621 = cJSON_GetObjectItem(key62, "key621");

    printf("result is: %d\n", key621->valueint);

}
