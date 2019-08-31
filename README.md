#Linux 内核  -------------------------------------
##虚拟内存的概念；
虚拟内存是计算机系统内存管理的一种技术。它使得应用程序认为它拥有连续的可用的内存（一个连续完整的地址空间），而实际上，它通常是被分隔成多个物理内存碎片，还有部分暂时存储在外部磁盘存储器上，在需要时进行数据交换。目前，大多数操作系统都使用了虚拟内存，如Windows家族的“虚拟内存”；Linux的“交换空间”等。虚拟内存技术主要有以下优点：虚拟内存可以大于物理内存，一般为物理内存的1.5倍到3倍，从而可以运行比物理内存大的程序，进而使得更多的程序可以同时执行，提高了多道程序的程度，增加了CPU的使用率，并且使得进程之间的独立性得到了更好的体现。

##中断；

#Linux 应用  -------------------------------------
##文件操作
###open
1，如果两个程序同时打开一个文件，会得到两个不同的文件描述符。
如果同时对两个文件进行操作，他们各自操作，互不影响，彼此相互覆盖（后写入的覆盖先写入的）。
为了防止文件按读写冲突，可以使用文件锁的功能。【因为文件描述符是进程的私有属性】
2，新文件描述符总是使用未用描述符的最小值。
3，运行中的程序能够一次打开的文件数目是有限制的。
这个限制由头文件limits.h中的OPEN_MAX常数定义，它随着系统地不同而不同，但POSIX规范要求它至少要为16。

    头文件：  
    #include <fcntl.h>  
    #include <sys/types.h>  
    #include <sys/stat.h>  
    
    函数原型：  
    int open(const char *path, int oflags);  
    int open(const char *path, int oflags, mode_t mode );  
    
    参数说明：  
    path：准备打开的文件或设备名字。  
    oflags：指出要打开文件的访问模式。  
        O_RDONLY  【3选1】以只读方式打开  
        O_WRONLY  【3选1】以只写方式打开  
        O_RDWR    【3选1】以读写方式打开  
        O_APPEDN  【|可选】以追加方式打开  
            O_TRUNC   【|可选】把文件长度设为零，丢弃以后的内容。  
            O_CREAT   【|可选】如果需要，就按参数mode中给出的访问模式创建文件。  
            O_EXCL    【|可选】与O_CREAT一起调用，确保调用者创建出文件。使用这个模式可防止两个程序同时创建一个文件，如果文件已经存在，open调用将失败。  
    
    mode：当使用O_CREAT标志的open来创建文件时，我们必须使用三个参数格式的open调用。第三个参数mode 是几个标志按位OR后得到的。  
        S_IRUSR：读权限，文件属主  
        S_IWUSR：写权限，文件属主  
        S_IXUSR：执行权限，文件属主  
        S_IRGRP：读权限，文件所属组  
        S_IWGRP：写权限，文件所属组  
        S_IXGRP：执行权限，文件所属组  
        S_IROTH：读权限，其他用户  
        S_IWOTH：写权限，其他用户  
        S_IXOTH：执行权限，其他用户 
###close
close系统调用用于“关闭”一个文件。文件描述符被释放，并能够重新使用。
当一个进程终止时，内核自动关闭它所有打开的文件。很多程序都利用了这一功能而不显式地用close关闭文件。

    头文件：  
    #include <unistd.h>  
    
    函数原型：  
    int close(int fd);  
    
    参数说明：  
    int：函数返回值close成功返回0，出错返回-1  

###read
从与文件描述符fd相关的文件中读取nbytes个字节的数据到buf中，返回实际读入的字节数：

    头文件：  
    #include <unistd.h>  
    
    函数原型  
    size_t read(int fd, void *buf, size_t nbytes);  
    
    参数说明：  
    fd：文件描述符，标识要读取的文件。如果为0，则从标准输入读数据。  
    buf：缓冲区，用来存储读入的数据。  
    nbytes：要读取的字符数。  
    size_t：返回值，返回成功读取的字符数，它可能会小于请求的字节数。-1表示出现错误。

###write
将字符串buf中前nbytes个字节的数据写入与文件描述符fd关联的文件中，返回实际写入的字节数：

    文件：  
    #include <unistd.h>  
    
    函数原型：  
    size_t write(int fd, const void *buf, size_t nbytes);  
    
    参数说明：  
    fd：文件描述符，目标文件。例如:fd的值为1，就向标准输出写数据，  
    buf：待写入的字符串。  
    nbytes：要写入的字符数。  
    size_t：函数返回值，返回成功写入文件的字符数。-1表示出现错误。

###ioctl 
系统ioctl用来提供一些与特定硬件设备有关的必要控制，所以它的用法随设备的不同而不同。例如：ioctl调用可以用于回绕磁带机或设置串行口的流控特性。因此，ioctl并不需要具备可移植性。此外，每个驱动程序都定义了自己的一组ioctl命令。

    为了向用户提供一个统一的接口，设备驱动程序封装了所有与硬件相关的特性。硬件的特有功能可通过ioctl完成。ioctl提供了一个用于控制设备及其描述符行为和配置底层服务的接口。终端、文件描述符、甚至磁带机都可以又为他们定义的ioctl。

    头文件：  
    #include <unistd.h>  
    
    函数原型：  
    int ioctl(int fd, int cmd,,,,,,); 
###lseek
lseek对文件描述符fd指定文件的读写指针进行设置，也就是说，它可以设置文件的下一个读写位置。
   
    头文件：  
    #include <unistd.h>  
    #include <sys/types.h>  
    
    函数原型：  
    off_t lseek(int filedes, off_t offset, int whence);  
    参数说明：  
    off_set：参数用来指定位置  
    whence：参数定义该偏移值的用法。  
        whence可以取下列值  
        SEEK_SET:offset是一个绝对位置  
        SEEK_CUR:offset是相对于当前位置的一个相对位置  
        SEEK_END:offset是相对于文件尾的一个相对位置  
    off_t:lseek返回从文件到文件指针被设置处的字节偏移，失败时返回-1.参数offset的类型off_t是一个与具体实现有关的整数类型，它定义在sys/types.h中  

###fstat stat lstat
fstat系统调用返回与打开的文件描述符相关的文件的状态信息，该信息将会写到buf结构中，buf的地址以参数形式传递给fstat。

    头文件：  
    #include <unistd.h>   
    #include <sys/stat.h>  
    #include <sys/types.h>  
    函数原型：  
    int fstat(int fildes, struct stat *buf);  
    int stat(const char *path, struct stat *buf);  
    int lstat(const char *path, struct stat *buf); 

相关的函数stat和lstat返回的是通过文件名查到的状态信息。它们的结果基本一致，但当文件是一个符号链接时，lstat返回的是该符号链接本身的信息，而stat返回的是该链接指向的文件的信息。stat结构的成员在不同的UNIX系统上会有所变化，但一般会包括表3-4中所示的内容。

    stat成员 说 明  
    st_mode 文件权限和文件类型信息  
    st_ino 与该文件关联的inode  
    st_dev 保存文件的设备  
    st_uid 文件属主的UID号  
    st_gid 文件属主的GID号  
    st_atime 文件上一次被访问的时间  
    st_ctime 文件的权限、属主、组或内容上一次被改变的时间  
    st_mtime 文件的内容上一次被修改的时间  
    st_nlink 该文件上硬连接的个数  

如果想测试一个文件“代表的不是一个目录，设置了属主的执行权限，不再有其他权限”，我们可以使用如下的代码进行测试：

    mode_t modes;  
    stat(“filename”,&statbuf);  
    modes = statbuf.st_mode;  
    if(!S_ISDIR(modes) && (modes & S_IRWXU) == S_IXUSR) 

stat结构中返回的st_mode标志还有一些与之关联的宏，它们定义在头文件sys/stat.h中。这些宏包括对访问权限、文件类型标志以及一些用于帮助测试特定类型和权限的掩码的定义。
访问权限标志与前面的open系统调用中的内容是一样的

    文件类型标志包括：  
    S_IFBLK：文件是一个特殊的块设备。  
    S_IFDIR：文件是一个目录。  
    S_IFCHR：文件是一个特殊的字符设备。  
    S_IFIFO：文件是一个FIFO设备（命名管道）  
    S_IFREG：文件是一个普通文件。  
    S_FLNK：文件是一个符号链接。  
    其他模式标志包括：  
    S_ISUID：文件设置了SUID位。  
    S_ISGID：文件设置了SGID位。  
    用于解释st_mode标志的掩码包括：  
    S_IFMT：文件类型。  
    S_IRWXU：属主的读/写/执行权限。  
    S_IRWXG：属组的读/写/执行权限。  
    S_IRWXO：其他用户的读/写/执行权限。  
还有一些用来帮助确定文件类型的宏定义。它们只是对经过掩码处理的模式标志和相应的设备类型标志进行比较。它们包括

    S_ISBLK：测试是否是特殊的块设备文件。  
    S_ISCHR：测试是否是特殊的字符设备文件。  
    S_ISDIR：测试是否是目录。  
    S_ISFIFO：测试是否是FIFO设备。  
    S_ISREG：测试是否是普通文件。  
    S_ISLNK：测试是否是符号链接。 

###dup dup2
dup，dup2系统调用。dup提供了复制文件描述符的方法，使我们能够通过两个或者更多个不同的文件描述符来访问同一个文件。这可以用于在文件的不同位置对数据进行读写。

    头文件：  
    #include <unistd.h>  
    
    dup系统调用复制文件描述符fildes，返回一个新的文件描述符。  
    int dup(int fildes);  
    dup2系统调用则是通过明确指定描述符来把一个文件描述符复制为另一个文件描述符  
    int dup2(int fildes,int fildes2);  
##库函数 
###fopen
读写文件通常是大量的数据（这种大量是相对于底层驱动的系统调用所实现的数据操作单位而言），这时，使用库函数就可以大大减少系统调用的次数。这一结果又缘于缓冲区技术。在用户空间和内核空间，对文件操作都使用了缓冲区，例如用fwrite写文件，都是先将内容写到用户空间缓冲区，当用户空间缓冲区满或者写操作结束时，才将用户缓冲区的内容写到内核缓冲区，同样的道理，当内核缓冲区满或写结束时才将内核缓冲区内容写到文件对应的硬件媒介。

    函数原型：  
    FILE *fopen(const char *filename, const char *mode);  
    
    参数说明：  
    filename：打开文件的文件名  
    mode：打开的方式  
        "r"或"rb":以只读方式打开  
        "w"或"wb":以只写方式打开  
        "a"或"ab":以读方式打开,添加到文件的结尾处  
        "r+"或"rb+"或"r+b":打开更新(读和写)  
        "w+"或"wb+"或"w+b":打开更新,将其长度变为零  
        "a+"或"ab+"或"a+b":打开更新,添加到文件结尾处  
        [注意]b表明这个文件是二进制文件而不是文本文件.  
    FILE：返回值，成功是返回一个非空的FILE *指针。失败返回NULL  
###fread

    函数原型：  
    size_t  fread(void *ptr, size_t size, size_t nitems, FILE *stream);  
    
    参数说明：  
    ptr： 要读取数据的缓冲区，也就是要存放读取数据的地方。  
    size：指定每个数据记录的长度。  
    nitems： 计数，给出要传输的记录个数。  
    size_t：返回值，成功读取到数据缓冲区的记录个数，当到达文件尾时，他的返回值可能会消耗与nitems，甚至可以是0  
###fwrite

    函数原型：  
    size_t  fwrite(const coid *ptr, size_t size , size_t nitimes, FILE *stream);  

###fclose
fclose库函数关闭指定的文件流stream，使所有尚未写出的数据都写出。因为stdio库会对数据进行缓冲，所以使用fclose是很重要的。如 果程序需要确保数据已经全部写出，就应该调用fclose函数。虽然当程序正常结束时，会自动对所有还打开的文件流调用fclose函数，但这样做就没有 机会检查由fclose报告的错误了。

    函数原型：  
    int fclose(FILE *stream);  
###fflush
fflush函数的作用是把文件流中所有未写出的数据全部写出。 处于效率考虑，在使用库函数的时候会使用数据缓冲区，当缓冲区满的时候才进行写操作。使用fflush函数可以将缓冲区的数据全部写出，而不关心缓冲区是否满。fclose的执行隐含调用了fflush函数，所以不必再fclose执行之前调用fflush

    函数原型：  
    int fflush(FILE *stream); 

###fseek

    函数原型：  
    int fseek(FILE *stream, long int offset, int whence);  
    参数说明：  
    off_set：参数用来指定位置  
    whence：参数定义该偏移值的用法。  
        whence可以取下列值  
        SEEK_SET:offset是一个绝对位置  
        SEEK_CUR:offset是相对于当前位置的一个相对位置  
        SEEK_END:offset是相对于文件尾的一个相对位置  
    int：函数返回值。0表示成功，-1表示失败。


###fgetc和getc和getchar

    函数原型：  
    int fgetc ( FILE * stream );   
    函数说明：  
    函数实现，从流stream中读一个字符。可以将标准输入stdin作为它的实参，这时候从标准输入读取一个字符。fgetc函数读字符时遇到文件结束符，函数返回一个文件结束标记EOF。  
    
    函数原型：  
    int getc(FILE * stream);       
    函数说明：  
    宏实现，宏实现版的fgetc  
    
    函数原型：  
    int getchar ( void );　　　　   
    函数说明：  
    宏实现，实现为#define getchar() fgetc(stdin)。  
    
    说明:getc、getchar都是通过宏定义借助fgetc实现。

###fgets gets

    函数原型：  
    char * fgets (char * str, int num, FILE *stream);  
    函数说明：  
    从流stream中读入最多num个字符到字符数组str中，当遇到换行符时、或读到num-1个字符时停止。自动加上'\0'空字符结尾  
    
    函数原型：  
    char * gets ( char * str );  
    函数说明：  
    从标准输入stdin读取一个字符串，遇到换行或结束时候终止。不同于fgets，他没有指定num，所以需要注意字符数组str的大小。 
###fputc和putc和putchar

    函数原型：  
    int fputc ( int character, FILE * stream );  
    函数说明：  
    把一个字符写到输出流。失败则返回EOF。  
    
    函数原型：  
    int putc ( int character, FILE * stream );         
    函数说明：  
    宏实现，通过宏定义和fputc实现  
    
    函数原型：  
    int putchar ( int character );　　　　　　　　  
    函数说明：  
    宏实现，#define putchar(c) fputc(c, stdout)  












    
    不能返回值；
    不能向ISR传递参数；
    ISR应该尽可能的短小精悍；
    printf(char *lpFormatString,...)函数会带来重入和性能问题题，不能再ISR中采用。（在ARM平台上由于半主机机制，该函数的速度更慢！）
    其实还包括所有的不可重入的函数都不应该在中断中使用。程序员应该仔细地评估ANSI C库函数和OS的系统调用
    浮点运算以及其他的消耗时间操作都不应该在中断服务程序中使用

###加快中断处理的方法

    中断中做最简短的操作，保留中断标志位，剩下的由中断标志位完成
#C 语言编程  -------------------------------------


预防重定义
Ifndef-- define-- endif

#数据结构
##树
    结点的度：结点拥有的子树的数目
    叶子结点：度为0的结点  终端节点，
    分支结点：度不为0的结点
    树的度：树中结点的最大的度
    层次：根结点的层次为1，其余结点的层次等于该结点的双亲结点的层次加1
    树的深度：树中结点的最大层次
    森林：0个或多个不相交的树组成。对森林加上一个根，森林即成为树；删去根，树即成为森林。
性质1：二叉树第i层上的结点数目最多为2i-1(i>=1)

性质2：深度为k的二叉树至多有2k-1个结点（k>=1）

性质3：包含n个结点的二叉树的高度至少为(log2n)+1

性质4：在任意一棵二叉树中，若终端结点的个数为n0，度为2的结点数为n2，则n0=n2+1
满二叉树  没有 度为1 的节点
完全二叉树 度为1 的节点只有0-1个

    n=n0+n1+n2  总节点等于 节点为 0 1 2 之和
    n=n1+2n2+1  总节点等于 度为 1 2倍度为2 加一

（1）若任意结点的左子树不空，则左子树上所有结点的值均小于它的根结点的值。
（2）任意结点的右子树不空，则右子树上所有结点的值均大于它的根结点的值。
（3）任意结点的左、右子树也分别为二叉查找树。
（4）没有键值相等的结点。
