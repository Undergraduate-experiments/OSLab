#include "x86.h"
#include "string.h"
enum i_mode_type {I_DIRECTORY = 0,I_CHAR_SPECIAL,I_CREAT_PROC};
enum power{read = 0,write};
#define NR_DEFAULT_FILE_SECS 4  //规定一个文件默认扇区大小为4
#define MAX_FILENAME_LEN     20 //文件名大小
#define MAX_FILE_NUMBER      200
#define MAX_DIR_NUMBER       200
struct super_block{
   uint32_t magic;             //文件系统的标识。这里用一个魔数表明本文件系统。
   uint32_t nr_inodes;         //4096 个
   uint32_t nr_sects;          //有多少个扇区(40257)
   uint32_t nr_imap_sects;     // inode_map 占用多少扇区(取值为1)。
   uint32_t nr_smap_sects;     // sector_map 占 用 多 少 扇 区(nr_sects/4096+1)
   uint32_t n_lst_sect;        //第一个数据扇区的扇区号是多少
   uint32_t root_inode;        //根目录区的 i-node 号是多少(取值为 1)
   uint32_t nr_inodes_sects;   //inodes 占用的扇区数
};

extern struct super_block super;

struct inode{
   uint32_t i_mode;             //访问方式
   uint32_t i_size;             // 文件大小
   uint32_t i_start_sect;       //第一个扇区
   uint32_t i_nr_sects;         //文件占用了多少扇区
   int      i_num;              //i-node 编号
};



struct inode inode_array[4096];
int    inode_map[4096];          //used == 1 ; unused == 0
//inode-map 用来映射 inode-array ,可以进行位操作。
int    sector_map[40257];       //used == 1 ; unused == 0

struct dir_entry{
    int inode_nr;                 //inode 编号
    char name[MAX_FILENAME_LEN];  //文件名
};

struct dir_entry dir_array[MAX_DIR_NUMBER];
extern int dir_entry_cnt ;



struct file_desc{
    int fd_mode;                    //read or write
    int fd_pos;                     //文件指针位置
    struct inode* fd_inode;         //指向 inode 的指针
};
struct file_desc file_table[MAX_FILE_NUMBER];
extern int sec_cnt;                 //创建文件可以使用的首个扇区号
extern int file_table_cnt;          //file_table可以使用的首个下标
void init_fs(struct super_block* super);
int  create_file(char filename[MAX_FILENAME_LEN]);
int new_inode();
void alloc_imap_bit(int new_inode);
void alloc_smap_bit();
void new_dir_entry(char filename[MAX_FILENAME_LEN],int new_inode);
