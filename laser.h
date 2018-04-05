/*
 *  `laser.h`
 *
 *  USAGE:
 *      #define LASER_IMPL
 *      #include "laser.h"
 *
 *      Configuration Options:
 *          #define LASER_ASSERT - Provide a custom `assert`, otherwise defaults to `assert` from <assert.h>.
 *
 *  LICENSE:
 *      See end of file for license information.
 *
 *  EXAMPLE:
 *      See `example/example.c` for a working example.
 *
 *  API:
 *      `laser` consists of two APIs:
 *          - Simple.
 *          - Granular.
 */

#if !defined(LASER_H)
#define LASER_H

#define LASER_VERSION_MAJOR 0
#define LASER_VERSION_MINOR 7
#define LASER_VERSION_PATCH 2

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(_MSC_VER)
#if _MSC_VER < 1300
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
#else
typedef signed __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef signed __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef signed __int32 int32_t;
typedef unsigned __int32 uint32_t;
#endif
typedef signed __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

#if defined(LASER_STATIC)
#define LASER_API static
#else
#define LASER_API extern
#endif

#if defined(__GNUC__)
#define LASER_DEPRECATED __attribute__((deprecated))
#elif defined(_WIN32)
#define LASER_DEPRECATED __declspec(deprecated)
#else
#define LASER_DEPRECATED
#endif

#define LASER_DEFAULT_STRIDE ((uint64_t) 0)
#define LASER_ALL_POINTS ((uint64_t) 0)

typedef enum laserResult {
    LASER_SUCCESS = 0,
    LASER_ERROR_INVALID_FILE = -1,
    LASER_ERROR_INVALID_RANGE = -2,
    LASER_ERROR_VERSION_UNSUPPORTED = -3,
    LASER_ERROR_FORMAT_UNSUPPORTED = -4,
    LASER_ERROR_IO_READ = -5
} laserResult;

LASER_API const char* laser_result_str(laserResult res);

typedef enum laserClassification {
    LASER_CLASSIFICATION_NEVER_CLASSIFIED,
    LASER_CLASSIFICATION_UNCLASSIFIED,
    LASER_CLASSIFICATION_GROUND,
    LASER_CLASSIFICATION_LOW_VEGETATION,
    LASER_CLASSIFICATION_MED_VEGETATION,
    LASER_CLASSIFICATION_HIGH_VEGETATION,
    LASER_CLASSIFICATION_BUILDING,
    LASER_CLASSIFICATION_LOW_POINT,
    LASER_CLASSIFICATION_MODEL_KEY_POINT,
    LASER_CLASSIFICATION_WATER,
    LASER_CLASSIFICATION_RESERVED_1,
    LASER_CLASSIFICATION_RESERVED_2,
    LASER_CLASSIFICATION_OVERLAP_POINTS,
    LASER_CLASSIFICATION_RESERVED_3,
    LASER_CLASSIFICATION_RESERVED_4,
    LASER_CLASSIFICATION_RESERVED_5,
    LASER_CLASSIFICATION_RESERVED_6,
    LASER_CLASSIFICATION_RESERVED_7,
    LASER_CLASSIFICATION_RESERVED_8,
    LASER_CLASSIFICATION_RESERVED_9,
    LASER_CLASSIFICATION_RESERVED_10,
    LASER_CLASSIFICATION_RESERVED_11,
    LASER_CLASSIFICATION_RESERVED_12,
    LASER_CLASSIFICATION_RESERVED_13,
    LASER_CLASSIFICATION_RESERVED_14,
    LASER_CLASSIFICATION_RESERVED_15,
    LASER_CLASSIFICATION_RESERVED_16,
    LASER_CLASSIFICATION_RESERVED_17,
    LASER_CLASSIFICATION_RESERVED_18,
    LASER_CLASSIFICATION_RESERVED_19,
    LASER_CLASSIFICATION_RESERVED_20
} laserClassification;

typedef struct laserInfo {
    uint8_t version_major;
    uint8_t version_minor;
    uint64_t point_count;
    uint32_t point_offset;
    uint32_t point_size;
    uint32_t point_format;
    float scale_x;
    float scale_y;
    float scale_z;
    float offset_x;
    float offset_y;
    float offset_z;
    float min_x;
    float min_y;
    float min_z;
    float max_x;
    float max_y;
    float max_z;
} laserInfo;

typedef struct laserPoint {
    float x, y, z;
    uint16_t intensity;
    struct {
        uint8_t return_number: 3;
        uint8_t return_count: 3;
        uint8_t scan_direction: 1;
        uint8_t edge: 1;
    } flags;
    struct {
        uint8_t type: 4;
        uint8_t synthetic: 1;
        uint8_t keypoint: 1;
        uint8_t withheld: 1;
        uint8_t _: 1;
    } classification;
    int8_t scan_angle;
    uint8_t usr;
    uint16_t point_id;
} laserPoint;

typedef uint64_t (*laserIoReadFn)(void* usr, void* data, uint64_t size, uint64_t offset);

/*
 *  Simple API - Supports reading the common subset of attributes shared between points formats: 0 - 5.
 *
 *  `laser_*_from_mem` - Requires the entire LAS file to be in memory.
 *  `laser_*_from_io` - Supports reading data on demand from the `io` callbacks.
 *
 *  Example:
 *      laserInfo info;
 *      laser_info_from_mem(&info, las_file_data, las_file_size);
 *      laserPoint* points = (laserPoint*) malloc(sizeof(laserPoint) * info.point_count);
 *      laserResult result = laser_read_from_mem(points, LASER_DEFAULT_STRIDE, las_file_data, las_file_size);
 */

LASER_API laserResult laser_info_from_mem(laserInfo* info, void* mem, uint64_t size);
LASER_API laserResult laser_read_from_mem(laserPoint* points, uint64_t stride, void* mem, uint64_t size);
LASER_API laserResult laser_read_range_from_mem(laserPoint* points, uint64_t stride, void* mem, uint64_t size, uint64_t first, uint64_t count);

LASER_API laserResult laser_info_from_io(laserInfo* info, laserIoReadFn fn, void* usr);
LASER_API laserResult laser_read_from_io(laserPoint* points, uint64_t stride, laserIoReadFn fn, void* usr);
LASER_API laserResult laser_read_range_from_io(laserPoint* points, uint64_t stride, laserIoReadFn fn, void* usr, uint64_t first, uint64_t count);

typedef enum laserAttribType {
    LASER_ATTRIB_TYPE_NONE = -1,

    LASER_ATTRIB_TYPE_X,                                /* float    */
    LASER_ATTRIB_TYPE_Y,                                /* float    */
    LASER_ATTRIB_TYPE_Z,                                /* float    */
    LASER_ATTRIB_TYPE_INTENSITY,                        /* uint16_t */
    LASER_ATTRIB_TYPE_FLAGS,                            /* uint8_t  */
    LASER_ATTRIB_TYPE_CLASSIFICATION,                   /* uint8_t  */
    LASER_ATTRIB_TYPE_SCAN_ANGLE,                       /* int8_t   */
    LASER_ATTRIB_TYPE_USR,                              /* uint8_t  */
    LASER_ATTRIB_TYPE_POINT_ID,                         /* uint16_t */
    /*
     * Unsupported:
     * LASER_ATTRIB_TYPE_GPS_TIME,
     * LASER_ATTRIB_TYPE_RED,
     * LASER_ATTRIB_TYPE_GREEN,
     * LASER_ATTRIB_TYPE_BLUE,
     * LASER_ATTRIB_TYPE_WAVEFORM_ID,
     * LASER_ATTRIB_TYPE_WAVEFORM_OFFSET,
     * LASER_ATTRIB_TYPE_WAVEFORM_SIZE,
     * LASER_ATTRIB_TYPE_WAVEFORM_LOCATION,
     * LASER_ATTRIB_TYPE_X_TIME,
     * LASER_ATTRIB_TYPE_Y_TIME,
     * LASER_ATTRIB_TYPE_Z_TIME,
     */
     LASER_ATTRIB_TYPE_COUNT
} laserAttribType;

typedef struct laserAttrib {
    laserAttribType type;
    uint64_t offset;
} laserAttrib;

#define LASER_ATTRIB_END { LASER_ATTRIB_TYPE_NONE, 0 }

/*
 *  Granular API - Supports reading any set of attributes into a user-defined layout.
 *
 *  `laser_*_from_mem_with_attribs` - Requires the entire LAS file to be in memory, reads specified attributes into their corresponding offset.
 *  `laser_*_from_io_with_attribs` - Supports reading data on demand from the `io` callbacks, reads specified attributes into their corresponding offset.
 *
 *  Example:
 *      laserInfo info;
 *      laser_info_from_mem(&info, las_file_data, las_file_size);
 *      float* points = (float*) malloc(sizeof(float) * 3 * info.point_count);
 *      laserAttrib attribs[] = {
 *          {  LASER_ATTRIB_TYPE_X, 0 },
 *          {  LASER_ATTRIB_TYPE_Y, 4 },
 *          {  LASER_ATTRIB_TYPE_Z, 8 },
 *          LASER_ATTRIB_END
 *      };
 *      laserResult result = laser_read_range_from_mem_with_attribs(points, 12, attribs, las_file_data, las_file_size, 0, LASER_ALL_POINTS);
 */

LASER_API laserResult laser_read_range_from_mem_with_attribs(void* points, uint64_t stride, laserAttrib* attribs, void* mem, uint64_t size, uint64_t first, uint64_t count);
LASER_API laserResult laser_read_range_from_io_with_attribs(void* points, uint64_t stride, laserAttrib* attribs, laserIoReadFn fn, void* usr, uint64_t first, uint64_t count);

#if defined(__cplusplus)
}
#endif

#endif

#if defined(LASER_IMPL)

#if !defined(LASER_ASSERT)
#include <assert.h>
#define LASER_ASSERT(x) assert(x)
#endif

#define LASER_OFFSET_OF(s, m) ((uint64_t) (&(((s*) 0)->m)))

static const uint8_t _LASER_MAGIC[4] = { 'L', 'A', 'S', 'F' };

#pragma pack(push, 1)
typedef struct laserPublicHeaderBlock {
    uint32_t magic;
    uint16_t file_source_id;
    struct {
        uint16_t gps_type: 1;
        uint16_t waveform_internal: 1;
        uint16_t waveform_external: 1;
        uint16_t synthetic_return_count: 1;
        uint16_t wkt: 1;
        uint16_t _: 11;
    } global_encoding;
    uint32_t guid_1;
    uint16_t guid_2;
    uint16_t guid_3;
    uint64_t guid_4;
    uint8_t version_major;
    uint8_t version_minor;
    char system_id[32];
    char software[32];
    uint16_t ddd;
    uint16_t yyyy;
    uint16_t phb_size;
    uint32_t point_offset;
    uint32_t vlr_count;
    uint8_t format_id;
    uint16_t point_size;
    uint32_t point_count;
    uint32_t point_count_per_return[5];
    double x_scale;
    double y_scale;
    double z_scale;
    double x_offset;
    double y_offset;
    double z_offset;
    double x_max;
    double x_min;
    double y_max;
    double y_min;
    double z_max;
    double z_min;
} laserPublicHeaderBlock;
#pragma pack(pop)

static const uint64_t _LASER_ATTRIB_OFFSET_TABLE[6][20] = {
    { 0, 4, 8, 12, 14, 15, 16, 17, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 4, 8, 12, 14, 15, 16, 17, 18, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 4, 8, 12, 14, 15, 16, 17, 18, 0, 20, 22, 24, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 4, 8, 12, 14, 15, 16, 17, 18, 20, 28, 30, 32, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 4, 8, 12, 14, 15, 16, 17, 18, 20, 0, 0, 0, 28, 29, 37, 41, 45, 49, 53 },
    { 0, 4, 8, 12, 14, 15, 16, 17, 18, 20, 28, 30, 32, 34, 35, 43, 47, 51, 55, 59 },
};

static const uint32_t _LASER_VALID_ATTRIB_TABLE[6] = {
    0x1FF, 0x3FF, 0x1DFF, 0x1FFF, 0xFE3FF, 0xFFFFF,
};

static laserAttrib _LASER_DEFAULT_ATTRIBS[] = {
    { LASER_ATTRIB_TYPE_X, LASER_OFFSET_OF(laserPoint, x) },
    { LASER_ATTRIB_TYPE_Y, LASER_OFFSET_OF(laserPoint, y) },
    { LASER_ATTRIB_TYPE_Z, LASER_OFFSET_OF(laserPoint, z) },
    { LASER_ATTRIB_TYPE_INTENSITY, LASER_OFFSET_OF(laserPoint, intensity) },
    { LASER_ATTRIB_TYPE_FLAGS, LASER_OFFSET_OF(laserPoint, flags) },
    { LASER_ATTRIB_TYPE_CLASSIFICATION, LASER_OFFSET_OF(laserPoint, classification) },
    { LASER_ATTRIB_TYPE_SCAN_ANGLE, LASER_OFFSET_OF(laserPoint, scan_angle) },
    { LASER_ATTRIB_TYPE_USR, LASER_OFFSET_OF(laserPoint, usr) },
    { LASER_ATTRIB_TYPE_POINT_ID, LASER_OFFSET_OF(laserPoint, point_id) },
    LASER_ATTRIB_END
};

static laserResult _laser_check_magic(uint8_t* magic) {
    return
        magic[0] != _LASER_MAGIC[0] ||
        magic[1] != _LASER_MAGIC[1] ||
        magic[2] != _LASER_MAGIC[2] ||
        magic[3] != _LASER_MAGIC[3] ? LASER_ERROR_INVALID_FILE: LASER_SUCCESS;
}

laserResult laser_info_from_mem(laserInfo* info, void* mem, uint64_t size) {
    (void) size;

    laserResult res = LASER_SUCCESS;
    if((res = _laser_check_magic((uint8_t*) mem)) != LASER_SUCCESS) {
        return res;
    }

    laserPublicHeaderBlock* public_header_block = (laserPublicHeaderBlock*) mem;
    if(public_header_block->version_major > 1 || (public_header_block->version_major == 1 && public_header_block->version_minor > 3)) {
        return LASER_ERROR_VERSION_UNSUPPORTED;
    } else if(public_header_block->format_id > 5) {
        return LASER_ERROR_FORMAT_UNSUPPORTED;
    }

    info->version_major = public_header_block->version_major;
    info->version_minor = public_header_block->version_minor;
    info->point_format = public_header_block->format_id;
    info->point_size = public_header_block->point_size;
    info->point_count = public_header_block->point_count;
    info->point_offset = public_header_block->point_offset;
    info->scale_x = (float) public_header_block->x_scale;
    info->scale_y = (float) public_header_block->y_scale;
    info->scale_z = (float) public_header_block->z_scale;
    info->offset_x = (float) public_header_block->x_offset;
    info->offset_y = (float) public_header_block->y_offset;
    info->offset_z = (float) public_header_block->z_offset;
    info->min_x = (float) public_header_block->x_min;
    info->max_x = (float) public_header_block->x_max;
    info->min_y = (float) public_header_block->y_min;
    info->max_y = (float) public_header_block->y_max;
    info->min_z = (float) public_header_block->z_min;
    info->max_z = (float) public_header_block->z_max;
    return LASER_SUCCESS;
}

laserResult laser_read_from_mem(laserPoint* points, uint64_t stride, void* mem, uint64_t size) {
    return laser_read_range_from_mem(points, stride, mem, size, 0, LASER_ALL_POINTS);
}

laserResult laser_read_range_from_mem(laserPoint* points, uint64_t stride, void* mem, uint64_t size, uint64_t first, uint64_t count) {
    return laser_read_range_from_mem_with_attribs(points, stride == LASER_DEFAULT_STRIDE ? sizeof(laserPoint): stride, _LASER_DEFAULT_ATTRIBS, mem, size, first, count);
}

laserResult laser_info_from_io(laserInfo* info, laserIoReadFn fn, void* usr) {
    laserPublicHeaderBlock public_header_block;
    uint32_t read = fn(usr, (void*) &public_header_block, sizeof(laserPublicHeaderBlock), 0);
    if(read == sizeof(laserPublicHeaderBlock)) {
        return laser_info_from_mem(info, (void*) &public_header_block, read);
    }
    return LASER_ERROR_IO_READ;
}

laserResult laser_read_from_io(laserPoint* points, uint64_t stride, laserIoReadFn fn, void* usr) {
    return laser_read_range_from_io(points, stride, fn, usr, 0, LASER_ALL_POINTS);
}

laserResult laser_read_range_from_io(laserPoint* points, uint64_t stride, laserIoReadFn fn, void* usr, uint64_t first, uint64_t count) {
    return laser_read_range_from_io_with_attribs(points, stride == LASER_DEFAULT_STRIDE ? sizeof(laserPoint): stride, _LASER_DEFAULT_ATTRIBS, fn, usr, first, count);
}

enum {
    LASER_ATTRIB_FLAG_X = (1 << 0),
    LASER_ATTRIB_FLAG_Y = (1 << 1),
    LASER_ATTRIB_FLAG_Z = (1 << 2),
    LASER_ATTRIB_FLAG_INTENSITY = (1 << 3),
    LASER_ATTRIB_FLAG_FLAGS = (1 << 4),
    LASER_ATTRIB_FLAG_CLASSIFICATION = (1 << 5),
    LASER_ATTRIB_FLAG_SCAN_ANGLE = (1 << 6),
    LASER_ATTRIB_FLAG_USR = (1 << 7),
    LASER_ATTRIB_FLAG_POINT_ID = (1 << 8),
    LASER_ATTRIB_FLAG_GPS_TIME = (1 << 9),
    LASER_ATTRIB_FLAG_RED = (1 << 10),
    LASER_ATTRIB_FLAG_GREEN = (1 << 11),
    LASER_ATTRIB_FLAG_BLUE = (1 << 12),
    LASER_ATTRIB_FLAG_WAVEFORM_ID = (1 << 13),
    LASER_ATTRIB_FLAG_WAVEFORM_OFFSET = (1 << 14),
    LASER_ATTRIB_FLAG_WAVEFORM_SIZE = (1 << 15),
    LASER_ATTRIB_FLAG_WAVEFORM_LOCATION = (1 << 16),
    LASER_ATTRIB_FLAG_X_TIME = (1 << 17),
    LASER_ATTRIB_FLAG_Y_TIME = (1 << 18),
    LASER_ATTRIB_FLAG_Z_TIME = (1 << 19)
};

static laserResult _laser_read_attribs_from_mem(void* points, uint64_t stride, uint32_t flags, uint64_t* offsets, laserInfo* info, void* raw_points, uint64_t size, uint64_t first, uint64_t count) {
    (void) size;

    count = count == LASER_ALL_POINTS ? info->point_count: count;
    if((first + count) > info->point_count) {
        return LASER_ERROR_INVALID_RANGE;
    }

    uint8_t* raw_point = ((uint8_t*) raw_points) + (first * info->point_size);
    uint8_t* point = (uint8_t*) points;
    uint64_t point_size = info->point_size;
    uint32_t point_format = info->point_format;
    float scale_x = info->scale_x;
    float scale_y = info->scale_y;
    float scale_z = info->scale_z;
    float offset_x = info->offset_x;
    float offset_y = info->offset_y;
    float offset_z = info->offset_z;
    const uint64_t* offset_table = _LASER_ATTRIB_OFFSET_TABLE[point_format];
    for(uint64_t i = 0; i < count; i++) {
        if(flags & LASER_ATTRIB_FLAG_X) {
            *((float*) (point + offsets[LASER_ATTRIB_TYPE_X])) = *((int32_t*) (raw_point + offset_table[LASER_ATTRIB_TYPE_X])) * scale_x + offset_x;
        }
        if(flags & LASER_ATTRIB_FLAG_Y) {
            *((float*) (point + offsets[LASER_ATTRIB_TYPE_Y])) = *((int32_t*) (raw_point + offset_table[LASER_ATTRIB_TYPE_Y])) * scale_y + offset_y;
        }
        if(flags & LASER_ATTRIB_FLAG_Z) {
            *((float*) (point + offsets[LASER_ATTRIB_TYPE_Z])) = *((int32_t*) (raw_point + offset_table[LASER_ATTRIB_TYPE_Z])) * scale_z + offset_z;
        }
        if(flags & LASER_ATTRIB_FLAG_INTENSITY) {
            *((uint16_t*) (point + offsets[LASER_ATTRIB_TYPE_INTENSITY])) = *((uint16_t*) (raw_point + offset_table[LASER_ATTRIB_TYPE_INTENSITY]));
        }
        if(flags & LASER_ATTRIB_FLAG_FLAGS) {
            *((uint8_t*) (point + offsets[LASER_ATTRIB_TYPE_FLAGS])) = *((uint8_t*) (raw_point + offset_table[LASER_ATTRIB_TYPE_FLAGS]));
        }
        if(flags & LASER_ATTRIB_FLAG_CLASSIFICATION) {
            *((uint8_t*) (point + offsets[LASER_ATTRIB_TYPE_CLASSIFICATION])) = *((uint8_t*) (raw_point + offset_table[LASER_ATTRIB_TYPE_CLASSIFICATION]));
        }
        if(flags & LASER_ATTRIB_FLAG_SCAN_ANGLE) {
            *((int8_t*) (point + offsets[LASER_ATTRIB_TYPE_SCAN_ANGLE])) = *((int8_t*) (raw_point + offset_table[LASER_ATTRIB_TYPE_SCAN_ANGLE]));
        }
        if(flags & LASER_ATTRIB_FLAG_USR) {
            *((uint8_t*) (point + offsets[LASER_ATTRIB_TYPE_USR])) = *((uint8_t*) (raw_point + offset_table[LASER_ATTRIB_TYPE_USR]));
        }
        if(flags & LASER_ATTRIB_FLAG_POINT_ID) {
            *((uint16_t*) (point + offsets[LASER_ATTRIB_TYPE_POINT_ID])) = *((uint16_t*) (raw_point + offset_table[LASER_ATTRIB_TYPE_POINT_ID]));
        }
        point += stride;
        raw_point += point_size;
    }
    return LASER_SUCCESS;
}

laserResult laser_read_range_from_mem_with_attribs(void* points, uint64_t stride, laserAttrib* attribs, void* mem, uint64_t size, uint64_t first, uint64_t count) {
    laserInfo info;
    laserResult res = LASER_SUCCESS;
    if((res = laser_info_from_mem(&info, mem, size)) != LASER_SUCCESS) {
        return res;
    }

    uint64_t offsets[LASER_ATTRIB_TYPE_COUNT];
    uint32_t flags = 0;
    for(uint64_t i = 0; i < LASER_ATTRIB_TYPE_COUNT && attribs[i].type != LASER_ATTRIB_TYPE_NONE; i++) {
        offsets[attribs[i].type] = attribs[i].offset;
        flags |= 1 << attribs[i].type;
    }
    return _laser_read_attribs_from_mem(points, stride, flags, offsets, &info, ((uint8_t*) mem) + info.point_offset, size, first, count);
}

laserResult laser_read_range_from_io_with_attribs(void* points, uint64_t stride, laserAttrib* attribs, laserIoReadFn fn, void* usr, uint64_t first, uint64_t count) {
    laserInfo info;
    laserResult res = LASER_SUCCESS;
    if((res != laser_info_from_io(&info, fn, usr)) != LASER_SUCCESS) {
        return res;
    }
    uint64_t offsets[LASER_ATTRIB_TYPE_COUNT];
    uint32_t flags = 0;
    for(uint64_t i = 0; i < LASER_ATTRIB_TYPE_COUNT && attribs[i].type != LASER_ATTRIB_TYPE_NONE; i++) {
        offsets[attribs[i].type] = attribs[i].offset;
        flags |= 1 << attribs[i].type;
    }

    uint8_t point_buffer[2048];

    count = count == LASER_ALL_POINTS ? info.point_count: count;
    uint64_t max_point_count = sizeof(point_buffer) / info.point_size;
    uint64_t point_count = count > max_point_count ? max_point_count: count;

    uint64_t read_count = count / max_point_count;
    read_count = !read_count ? 1: read_count;

    uint64_t expected = info.point_size * point_count;
    for(uint64_t i = 0; i < read_count; i++) {
        uint64_t offset = info.point_offset + ((i + first) * info.point_size * point_count);
        uint64_t read = fn(usr, (void*) point_buffer, expected, offset);
        if(read == expected) {
            res = _laser_read_attribs_from_mem(((uint8_t*) points) + (i * point_count), stride, flags, offsets, &info, (void*) point_buffer, read, 0, point_count);
            if(res != LASER_SUCCESS) {
                return res;
            }
        } else {
            return LASER_ERROR_IO_READ;
        }
    }
    return LASER_SUCCESS;
}

const char* laser_result_str(laserResult res) {
    switch(res) {
        case LASER_SUCCESS: return "Success";
        case LASER_ERROR_INVALID_FILE: return "Unknown file format";
        case LASER_ERROR_INVALID_RANGE: return "Invalid point range";
        case LASER_ERROR_VERSION_UNSUPPORTED: return "Unsupported version, supported versions: 1.0, 1.1, 1.2 and 1.3";
        case LASER_ERROR_FORMAT_UNSUPPORTED: return "Unknown point format, known formats: 0, 1, 2, 3, 4 and 5";
        case LASER_ERROR_IO_READ: return "Truncated read";
        default: return "Unknown error";
    }
}

#endif

/*
 * ---------------------------------- LICENSE ----------------------------------
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org>
 * =============================================================================
 */
