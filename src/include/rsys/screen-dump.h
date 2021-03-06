#if !defined (__rsys_screen_dump_h__)
#  define __rsys_screen_dump_h__

struct PACKED header
{
  int16 byte_order;
  int16 magic_number;
  int32 ifd_offset;
};

struct PACKED directory_entry
{
  int16 tag;
  int16 type;
  int32 count;
  int32 value_offset;
};

struct PACKED ifd
{
  int16 count;
  struct directory_entry entries[1];
};

#endif /* !defined (__rsys_screen_dump_h__) */
