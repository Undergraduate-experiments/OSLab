#include "file.h"
struct super_block super;
int dir_entry_cnt = 0 ;
int sec_cnt = 5 ;
int file_table_cnt = 0;
void init_fs(struct super_block* super)
{
/*---------------------------------------建立超级块--------------------------------------------------------------------*/
      super->magic           = 1314;
      super->nr_inodes       = 4096;
      super->nr_sects        = 40257;             //有多少个扇区(40257)
      super->nr_imap_sects   = 1;                 // inode_map 占用多少扇区(取值为1)。
      super->nr_smap_sects   = 40257 / 4096 + 1;  // sector_map 占 用 多 少 扇 区(nr_sects/4096+1)
      super->n_lst_sect      = 1 + 1 + 1 + 40257 / 4096 + 1 + 4096 * sizeof(struct inode) / 512 ;       //第一个数据扇区的扇区号是多少
      super->root_inode      = 1;       //根目录区的 i-node 号是多少(取值为 1)
      super->nr_inodes_sects = 4096 * sizeof(struct inode) / 512 ;   //inodes 占用的扇区数    

/*---------------------------------------建立inode_map与初始化----------------------------------------------------------*/
      int i = 0 ;
      for(i = 0 ; i < 4096 ; ++i)
      {  // i = 2 3 4 is for dev1-3
        if(i == 0 || i == 1 || i == 2 || i == 3 || i == 4)  {inode_map[i] = 1;}
        else   {inode_map[i]  = 0 ;}
      }

/*----------------------------------------建立 inode_array--------------------------------------------------------------*/
     //ROOT_INODE
     inode_array[1].i_mode = I_DIRECTORY  ;                      //访问方式
     inode_array[1].i_size = sizeof(struct dir_entry) * 4 ;      // 文件大小
     inode_array[1].i_start_sect = 1 ;                           //第一个扇区
     inode_array[1].i_nr_sects   = NR_DEFAULT_FILE_SECS;        //文件占用了多少扇区
     inode_array[1].i_num = 1 ;                                  //i-node 编号     

     //dev-0
     inode_array[2].i_mode = I_CHAR_SPECIAL;                        //访问方式
     inode_array[2].i_size = 0 ;                                    //文件大小
     inode_array[2].i_start_sect = 0 ;                              //设备的设备号
     inode_array[2].i_nr_sects   = 0;                              //文件占用了多少扇区
     inode_array[2].i_num = 2 ;                                     //i-node 编号   


     //dev-1
     inode_array[3].i_mode = I_CHAR_SPECIAL  ;                      //访问方式
     inode_array[3].i_size = 0 ;                                    //文件大小
     inode_array[3].i_start_sect = 0 ;                              //设备的设备号
     inode_array[3].i_nr_sects   = 1;                              //文件占用了多少扇区
     inode_array[3].i_num = 3 ;                                     //i-node 编号  
 
     //dev-2
     inode_array[4].i_mode = I_CHAR_SPECIAL  ;                      //访问方式
     inode_array[4].i_size = 0 ;                                    //文件大小
     inode_array[4].i_start_sect = 2 ;                              //设备的设备号
     inode_array[4].i_nr_sects   = 0;                             //文件占用了多少扇区
     inode_array[4].i_num = 4 ;                                     //i-node 编号  
/*-----------------------------------------建立 sector-map----------------------------------------------------------------*/
    for(i = 0 ; i < 40257 ; ++i)
    {
      //目前共设置(NR_DEFAULT_FILES_SECTS+1)个扇区位置 1
      if(i == 0 || i == 1 || i == 2 || i == 3 || i == 4) {sector_map[i] = 1;}
      else  {sector_map[i] = 0 ;}
    }
/*------------------------------------------建立根目录--------------------------------------------------------------------*/    
    dir_array[0].inode_nr = 1;                          //inode 编号
    memcpy(dir_array[0].name,"/",10);                   //文件名

    dir_array[1].inode_nr = 2;                           //inode 编号
    memcpy(dir_array[1].name,"dev_tty0",10);             //设备名

    dir_array[2].inode_nr = 3;                           //inode 编号
    memcpy(dir_array[2].name,"dev_tty1",10);             //设备名

    dir_array[3].inode_nr = 4;                           //inode 编号
    memcpy(dir_array[3].name,"dev_tty2",10);            //设备名
    
    dir_entry_cnt = 4 ;
/*----------------------------------------filp数组初始化--------------------------------------------------------------------*/
    file_table[0].fd_mode  = read;
    file_table[0].fd_pos   = 0;
    file_table[0].fd_inode = &inode_array[2];

    file_table[1].fd_mode  = read;
    file_table[1].fd_pos   = 1;
    file_table[1].fd_inode = &inode_array[3];

    file_table[2].fd_mode  = read;
    file_table[2].fd_pos   = 2;
    file_table[2].fd_inode = &inode_array[4];
   
    file_table_cnt = 3;
}

int  create_file(char filename[MAX_FILENAME_LEN])
{
    //为文件内容(数据)分配扇区
    sec_cnt = sec_cnt + NR_DEFAULT_FILE_SECS ;
    //在 inode_array 中分配一个 i-node( new_inode())
    int new_inodes = new_inode();
    alloc_imap_bit(new_inodes);
    //在 sector-map 中分配一位或多位 (alloc_smap_bit())
    alloc_smap_bit();
    //在相应目录中写入一个目录项(dir_entry)(new_dir_entry())
    new_dir_entry(filename,new_inodes);
    return new_inodes;
}

int new_inode()
{
     int i = 0 ;
     int inode_number = 0;
     for(i = 0 ; i < 4096 ; ++i) {
        if(inode_map[i]  == 0)  {inode_number = i ;break;}
     }
     inode_array[inode_number].i_mode = I_CREAT_PROC ;                      //访问方式
     inode_array[inode_number].i_size = sizeof(struct dir_entry) * 4 ;      // 文件大小
     inode_array[inode_number].i_start_sect = sec_cnt - 4 ;                 //第一个扇区
     inode_array[inode_number].i_nr_sects   = NR_DEFAULT_FILE_SECS;        //文件占用了多少扇区
     inode_array[inode_number].i_num = inode_number;                       //i-node 编号  
     return inode_number;
}

void alloc_imap_bit(int new_inode)
{
     inode_map[new_inode] = 1;
}

void alloc_smap_bit()
{
     int i = 0 ;
     for(i = 0 ; i < NR_DEFAULT_FILE_SECS ; ++i)
     {
       sector_map[sec_cnt-1-i] = 1;
     }
}

void new_dir_entry(char filename[MAX_FILENAME_LEN],int new_inode)
{
    dir_array[dir_entry_cnt].inode_nr = new_inode;                   //inode 编号
    memcpy(dir_array[dir_entry_cnt++].name,filename,10);              //文件名   
}
