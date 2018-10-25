#include "hdf5.h"

#include <assert.h>
#include <string.h>

#define BUF_SIZE      2048

#define NAMENODE_HOST "jelly.ad.hdfgroup.org"
#define NAMENODE_PORT 8020

/* Converts an H5O_type_t enumerator into a character code */

char obj_type(H5O_type_t t)
{
  char result = '?';
  switch (t)
    {
    case H5O_TYPE_GROUP:
      result = 'G';
      break;
    case H5O_TYPE_DATASET:
      result = 'D';
      break;
    case H5O_TYPE_NAMED_DATATYPE:
      result = 'T';
      break;
    default:
        break;
    }
  return result;
}

/* H5Ovisit callback: prints the file name and object type code */

herr_t obj_count_cb
(
 hid_t             obj,
 const char       *name,
 const H5O_info_t *object_info,
 void             *op_data
 )
{
  const char* file_name = (char*) op_data;
  printf("%s\t%c\n", file_name, obj_type(object_info->type));
  return 0;
}

/*
 *
 * For each HDF5 file report the types of objects in the file.
 *
 */

void mapit(const char* file_name)
{
  H5FD_hdfs_fapl_t param;
  hid_t            fapl, file;
  hsize_t          size;
  size_t           obj_count;

  if ((fapl = H5Pcreate(H5P_FILE_ACCESS)) >= 0)
    {
      param.version = 1;
      strcpy(param.namenode_name, NAMENODE_HOST);
      param.namenode_port = NAMENODE_PORT;
      param.stream_buffer_size = 4096;
      assert(H5Pset_fapl_hdfs(fapl, &param) >= 0);

      if ((file = H5Fopen(file_name, H5F_ACC_RDONLY, fapl)) >= 0)
        {
          assert(H5Fget_filesize(file, &size) >= 0);
          assert(H5Ovisit(file,
                          H5_INDEX_NAME,
                          H5_ITER_NATIVE,
                          &obj_count_cb,
                          (void*) file_name) >= 0);
          assert(H5Fclose(file) >= 0);
        }
      assert(H5Pclose(fapl) >= 0);
    }
}

/* Go! */

void main(int argc, char** argv)
{
  char             buffer[BUF_SIZE];
  size_t           len;

  while(fgets(buffer, BUF_SIZE - 1, stdin))
    {
      len = strlen(buffer);
      if(len > 0 && buffer[len-1] == '\n')
        {
          buffer[len-1] = 0;
          mapit(buffer);
        }
    }

  return;
}
