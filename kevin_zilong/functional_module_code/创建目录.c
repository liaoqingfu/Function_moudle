int zx_create_dir(const char *path)
{
    int i,len;
    char DirName[128];
    if (path)
    {
        if( access(path, 0) == 0 )  //存在了
        {
            return 0;
        }
        memset(DirName, 0, 128);
        len = strlen(path);
        if (len > 128)
        {
            return -1;
        }
        strcpy(DirName, path);
        if(DirName[len-1] != '/')
        {
            strcat(DirName, "/");
        }

        len = strlen(DirName);
        for( i=1; i<len; i++ )
        {
            if( DirName[i] == '/' )
            {
                DirName[i] = '\0';
                if( access(DirName, 0) != 0 )
                {
                    if (mkdir( DirName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) == -1)
                    {
                        //dzlog_error("cat't mkdir %s, %s", DirName, strerror(errno));
                        return -1;
                    }
                    dzlog_info("mkdir %s", DirName);
                }
                DirName[i] = '/';
            }
        }
        return 0;
    }

    return -1;
}