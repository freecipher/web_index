/*
 * Copyright    : FreeCipher [www.freecipher.com]
 * Filename     : web_index.c
 * Anthor       : Gerald Suen <greald.suen@gmail.com>
 * Description  :Automatically create index.html file for Linux
 * Others       :
 * Version      : 1.0.0
 * Date         : 2020-01-13 16:34:43
 * History      :
 *
 * Date         :
 * Author       :
 * Modification :
 */

/*
 * 版权所有 : FreeCipher [www.freecipher.com]
 * 文件名称 : web_index.c
 * 文件作者 : Gerald Suen <greald.suen@gmail.com>
 * 描述说明 : 自动在空文件夹内生成 index.html 文件
 * 其他说明 :
 * 版本信息 : 1.0.0
 * 日期时间 : 2020-01-13 16:34:43
 * 修改记录 :
 *
 * 修改时间 :
 * 修改作者 :
 * 修改内容 :
 */

#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

void file_write(const char *path, const char *buff, size_t buff_len)
{
    if(path == NULL || buff == NULL || buff_len == 0)
    {
        return;
    }

    FILE *file = fopen(path,"w");
    if (file == NULL)
    {
        return;
    }

    size_t filebuff_use_len = 0;
    while (1)
    {
        int ret = fwrite(buff + filebuff_use_len, 1, buff_len - filebuff_use_len, file);
        if (ret < 0)
        {
            break;
        }
        else if (ret == 0)
        {
            break;
        }
        else
        {
            filebuff_use_len += ret;
        }

        if (filebuff_use_len == buff_len)
        {
            break;
        }

        fflush(file);
        fsync(fileno(file));
    }

    fclose(file);
}

void printdir(char *dir, int depth)
{
    int result = 0;
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    if((dp = opendir(dir)) == NULL)
    {
        // fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while ((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name,&statbuf);
        if (S_ISDIR(statbuf.st_mode))
        {
            /* Found a directory, but ignore . and .. */
            if (strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
            {
                continue;
            }
            
            // printf("%*s%s/\n",depth,"",entry->d_name);
            /* Recurse at a new indent level */
            printdir(entry->d_name,depth+4);
        }
        else
        {
            // printf("%*s%s\n",depth,"",entry->d_name);
            
            if (strcmp(entry->d_name, "index.php") == 0)
            {
                result = 1;
            }
            else if (strcmp(entry->d_name, "index.html") == 0)
            {
                result = 1;
            }
            else
            {
            }
        }
    }
    if (result == 0)
    {
        const char *str = "<!DOCTYPE html>\n"
                    "<html>\n"
                    "   <head>\n"
                    "       <title>403 Forbidden</title>\n"
                    "   </head>\n"
                    "   <body>\n"
                    "       <p>Directory access is forbidden.</p>\n"
                    "   </body>\n"
                    "</html>\n";
        file_write("index.html", str, strlen(str));
    }
    
    chdir("..");
    closedir(dp);
}

int main(int argc, char* argv[])
{
    char *topdir, pwd[2]=".";
    if (argc != 2)
    {
        topdir=pwd;
    }
    else
    {
        topdir=argv[1];
    }

    printf("Directory scan of %s\n",topdir);
    printdir(topdir,0);
    printf("done.\n");

    return 0;
}


