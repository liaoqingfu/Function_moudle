unsigned int get_system_tf_free(unsigned int *ptotalsize)
{
    struct statfs diskInfo;
    int freeMbyte = 0;
    if (ptotalsize != NULL)
    {
        *ptotalsize = 0;
    }
    if(access(LOCAL_SAVE_DIR, W_OK) == 0)
    {
        if (statfs(LOCAL_SAVE_DIR, &diskInfo) != -1)
        {
            unsigned long long blocksize     = diskInfo.f_bsize; // 每个block里包含的字节数
            unsigned long long totalsize     = blocksize * diskInfo.f_blocks;   // 文件系统数据块总数
            unsigned long long availableDisk = blocksize * diskInfo.f_bavail;   // 非root用户可获取的块数
          //unsigned long long freeDisk      = blocksize * diskInfo.f_bfree;    // 可用块数,root用户多占5%
            freeMbyte = availableDisk>>20;
            if (ptotalsize != NULL)
            {
                *ptotalsize = totalsize>>20;
            }
        }
    }
    return freeMbyte;
}