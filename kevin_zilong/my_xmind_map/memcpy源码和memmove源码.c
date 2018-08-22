 memcpy中dest和source中的区域不能重叠，否则会出现未知结果中dest和source中的区域不能重叠，否则会出现未知结果。函数没做
 
任何内存的处理，内存是否重叠由程序员自己控制。

memmove里面则判断了内存重叠的情况，当内存重叠时从后往前复制，以确保复制正常处理。 



不能对void指针进行算法操作，即不能对void指针进行如p++的操作,所以需要转换为具体的类型指针来操作,例如char *
void * __cdecl memcpy (
        void * dst,
        const void * src,
        size_t count
        )
{
        void * ret = dst;
        while (count--) {
                *(char *)dst = *(char *)src;
                dst = (char *)dst + 1;
                src = (char *)src + 1;
        }
        return(ret);
}

memmove源码实现
void* memmove(void* dst,const void* src,size_t count)
{
void* ret = dst;
//dst <= src表示，如果dst在src的前面，从前往后复制不会覆盖src中还没有复制的内容

if (dst <= src || (char*)dst >= ((char*)src + count))
{
//从前往后复制，则不会出现覆盖src中没有复制的内容
while(count--)
{
*(char*)dst = *(char*)src; //char类型指针，表示一个字节一个字节的复制
dst = (char*)dst + 1; //移动一个字节
src = (char*)src + 1;
}
}
else
{
//从后往前复制，则不会出现覆盖src中没有复制的内容
dst = (char*)dst + count - 1;//移动到末尾
src = (char*)src + count - 1;
while(count--)
{
*(char*)dst = *(char*)src;
dst = (char*)dst - 1; //移动一个字节
src = (char*)src - 1;
}
}
//返回dst的头指针，还方便左值操作。
//如：ptstr = memmove(ptstr,src,count); cout << memmove(ptstr,src,count);
return ret;
}

转换为具体的类型指针来操作,例如char *后，在进行算法操作
上面类似源代码的实现存在多次类型转换，所以，它还是可以进行优化的，优化后的代码如下：
void* memmove(void* dst,const void* src,size_t count)
{
assert(NULL !=src && NULL !=dst);
char* tmpdst = (char*)dst;
char* tmpsrc = (char*)src;

if (tmpdst <= tmpsrc || tmpdst >= tmpsrc + count)
{
while(count--)
{
*tmpdst++ = *tmpsrc++;
}
}
else
{
tmpdst = tmpdst + count - 1;
tmpsrc = tmpsrc + count - 1;
while(count--)
{
*tmpdst-- = *tmpsrc--;
}
}

return dst;
}
