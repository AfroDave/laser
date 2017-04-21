/*
 * `laser.h`
 *
 * usage:
 *      #define LASER_IMPL
 *      #include "laser.h"
 */

#if !defined(LASER_H)
#define LASER_H

#define LASER_VERSION_MAJOR 0
#define LASER_VERSION_MINOR 4
#define LASER_VERSION_PATCH 1

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef _MSC_VER
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

typedef enum laserError {
    LASER_ERROR_NONE = 0,
    LASER_ERROR_INVALID_FILE = -1,
    LASER_ERROR_INVALID_RANGE = -2,
    LASER_ERROR_VERSION_UNSUPPORTED = -3,
    LASER_ERROR_FORMAT_UNSUPPORTED = -4,
    LASER_ERROR_IO_READ = -5
} laserError;

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
    LASER_CLASSIFICATION_RESERVED_20,
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
    } classification;
    int8_t scan_angle;
    uint8_t usr;
    uint16_t point_id;
} laserPoint;

LASER_API laserError laser_info_from_mem(laserInfo* info, void* mem, uint64_t size);

LASER_API laserError laser_read_from_mem(laserPoint* points, uint64_t stride, void* mem, uint64_t size);
LASER_API laserError laser_read_range_from_mem(laserPoint* points, uint64_t stride, void* mem, uint64_t size, uint64_t first, uint64_t count);

typedef struct laserIo {
    uint32_t (*read)(void* usr, void* data, uint32_t size, uint64_t offset);
    uint32_t (*write)(void* usr, void* data, uint32_t size, uint64_t offset);
} laserIo;

LASER_API laserError laser_info_from_io(laserInfo* info, laserIo* io, void* usr);

LASER_API laserError laser_read_from_io(laserPoint* points, uint64_t stride, laserIo* io, void* usr);
LASER_API laserError laser_read_range_from_io(laserPoint* points, uint64_t stride, laserIo* io, void* usr, uint64_t first, uint64_t count);

LASER_API const char* laser_error_str(laserError err);

typedef enum laserAttribType {
    LASER_ATTRIB_TYPE_NONE = -1,
    LASER_ATTRIB_TYPE_X,
    LASER_ATTRIB_TYPE_Y,
    LASER_ATTRIB_TYPE_Z,
    LASER_ATTRIB_TYPE_INTENSITY,
    LASER_ATTRIB_TYPE_FLAG,
    LASER_ATTRIB_TYPE_CLASSIFICATION,
    LASER_ATTRIB_TYPE_SCAN_ANGLE,
    LASER_ATTRIB_TYPE_USR,
    LASER_ATTRIB_TYPE_POINT_ID,
    LASER_ATTRIB_TYPE_GPS_TIME,
    LASER_ATTRIB_TYPE_RED,
    LASER_ATTRIB_TYPE_GREEN,
    LASER_ATTRIB_TYPE_BLUE,
    LASER_ATTRIB_TYPE_WAVEFORM_ID,
    LASER_ATTRIB_TYPE_WAVEFORM_OFFSET,
    LASER_ATTRIB_TYPE_WAVEFORM_SIZE,
    LASER_ATTRIB_TYPE_WAVEFORM_LOCATION,
    LASER_ATTRIB_TYPE_X_TIME,
    LASER_ATTRIB_TYPE_Y_TIME,
    LASER_ATTRIB_TYPE_Z_TIME,
    LASER_ATTRIB_TYPE_COUNT
} laserAttribType;

#if 0
typedef struct laserAttrib {
    laserAttribType type;
    uint64_t offset;
} laserAttrib;

#define LASER_ATTRIB_END { LASER_ATTRIB_TYPE_NONE, 0 }

LASER_API laserError laser_read_attribs_from_mem(void* points, uint64_t stride, laserAttrib* attribs, void* mem, uint64_t size);

LASER_API laserError laser_read_attribs_from_io(void* points, uint64_t stride, laserAttrib* attribs, laserIo* io, void* usr);
#endif

#if defined(__cplusplus)
}
#endif

#endif

#if defined(LASER_IMPL)

#if !defined(LASER_ASSERT)
#include <assert.h>
#define LASER_ASSERT(x) assert(x)
#endif

#if defined(LASER_MEMSET)
#elif !defined(LASER_MEMSET)
#else
#error "LASER_MEMSET must be defined."
#endif

#if !defined(LASER_MEMSET)
#include <string.h>
#define LASER_MEMSET(s, c, sz) memset((s), (c), (sz))
#endif

static const uint8_t LASER_MAGIC[4] = { 'L', 'A', 'S', 'F' };

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

typedef struct laserPointDataRecord0 {
    int32_t x;
    int32_t y;
    int32_t z;
    uint16_t intensity;
    uint8_t flag;
    uint8_t classification;
    int8_t scan_angle;
    uint8_t usr;
    uint16_t point_id;
} laserPointDataRecord0;

typedef struct laserPointDataRecord1 {
    int32_t x;
    int32_t y;
    int32_t z;
    uint16_t intensity;
    uint8_t flag;
    uint8_t classification;
    int8_t scan_angle;
    uint8_t usr;
    uint16_t point_id;
    double gps_time;
} laserPointDataRecord1;

typedef struct laserPointDataRecord2 {
    int32_t x;
    int32_t y;
    int32_t z;
    uint16_t intensity;
    uint8_t flag;
    uint8_t classification;
    int8_t scan_angle;
    uint8_t usr;
    uint16_t point_id;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} laserPointDataRecord2;

typedef struct laserPointDataRecord3 {
    int32_t x;
    int32_t y;
    int32_t z;
    uint16_t intensity;
    uint8_t flag;
    uint8_t classification;
    int8_t scan_angle;
    uint8_t usr;
    uint16_t point_id;
    double gps_time;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} laserPointDataRecord3;

typedef struct laserPointDataRecord4 {
    int32_t x;
    int32_t y;
    int32_t z;
    uint16_t intensity;
    uint8_t flag;
    uint8_t classification;
    int8_t scan_angle;
    uint8_t usr;
    uint16_t point_id;
    double gps_time;
    uint8_t waveform_idx;
    uint64_t waveform_offset;
    uint32_t waveform_size;
    float waveform_location;
    float time_x;
    float time_y;
    float time_z;
} laserPointDataRecord4;

typedef struct laserPointDataRecord5 {
    int32_t x;
    int32_t y;
    int32_t z;
    uint16_t intensity;
    uint8_t flag;
    uint8_t classification;
    int8_t scan_angle;
    uint8_t usr;
    uint16_t point_id;
    double gps_time;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint8_t waveform_idx;
    uint64_t waveform_offset;
    uint32_t waveform_size;
    float waveform_location;
    float time_x;
    float time_y;
    float time_z;
} laserPointDataRecord5;

typedef struct laserPointDataRecord6 {
    int32_t x;
    int32_t y;
    int32_t z;
    uint16_t intensity;
    uint16_t flag;
    uint8_t classification;
    uint8_t usr;
    int16_t scan_angle;
    uint16_t point_id;
    double gps_time;
} laserPointDataRecord6;

typedef struct laserPointDataRecord7 {
    int32_t x;
    int32_t y;
    int32_t z;
    uint16_t intensity;
    uint16_t flag;
    uint8_t classification;
    uint8_t usr;
    int16_t scan_angle;
    uint16_t point_id;
    double gps_time;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} laserPointDataRecord7;

typedef struct laserPointDataRecord8 {
    int32_t x;
    int32_t y;
    int32_t z;
    uint16_t intensity;
    uint16_t flag;
    uint8_t classification;
    uint8_t usr;
    int16_t scan_angle;
    uint16_t point_id;
    double gps_time;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t near_infrared;
} laserPointDataRecord8;

typedef struct laserPointDataRecord9 {
    int32_t x;
    int32_t y;
    int32_t z;
    uint16_t intensity;
    uint16_t flag;
    uint8_t classification;
    uint8_t usr;
    int16_t scan_angle;
    uint16_t point_id;
    double gps_time;
    uint8_t waveform_idx;
    uint64_t waveform_offset;
    uint32_t waveform_size;
    float waveform_location;
    float time_x;
    float time_y;
    float time_z;
} laserPointDataRecord9;

typedef struct laserPointDataRecord10 {
    int32_t x;
    int32_t y;
    int32_t z;
    uint16_t intensity;
    uint16_t flag;
    uint8_t classification;
    uint8_t usr;
    int16_t scan_angle;
    uint16_t point_id;
    double gps_time;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t near_infrared;
    uint8_t waveform_idx;
    uint64_t waveform_offset;
    uint32_t waveform_size;
    float waveform_location;
    float time_x;
    float time_y;
    float time_z;
} laserPointDataRecord10;

typedef struct laserVlr {
    uint16_t reserved;
    char user_id[16];
    uint16_t record_id;
    uint16_t record_size;
    char desc[32];
} laserVlr;

typedef struct laserEvlr {
    uint16_t reserved;
    char user_id[16];
    uint16_t record_id;
    uint64_t record_size;
    char desc[32];
} laserEvlr;
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

static laserError _laser_check_magic(uint8_t* magic) {
    laserError res = LASER_ERROR_NONE;
    if(magic[0] != LASER_MAGIC[0] || magic[1] != LASER_MAGIC[1] ||
       magic[2] != LASER_MAGIC[2] || magic[3] != LASER_MAGIC[3]) {
        res = LASER_ERROR_INVALID_FILE;
    }
    return res;
}

laserError laser_info_from_mem(laserInfo* info, void* mem, uint64_t size) {
    (void) size;

    laserError res = LASER_ERROR_NONE;
    LASER_MEMSET(info, 0, sizeof(laserInfo));
    if((res = _laser_check_magic((uint8_t*) mem)) == LASER_ERROR_NONE) {
        laserPublicHeaderBlock* public_header_block = (laserPublicHeaderBlock*) mem;
        if(public_header_block->version_major > 1 || (public_header_block->version_major == 1 && public_header_block->version_minor > 3)) {
            res = LASER_ERROR_VERSION_UNSUPPORTED;
        } else if(public_header_block->format_id > 5) {
            res = LASER_ERROR_FORMAT_UNSUPPORTED;
        } else {
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
        }
    }
    return res;
}

#define LASER_ALL_POINTS ((uint64_t) 0)

static laserError _laser_read_from_mem(laserPoint* points, uint64_t stride, laserInfo* info, void* raw_points, uint64_t size, uint64_t first, uint64_t count) {
    (void) size;

    laserError res = LASER_ERROR_NONE;
    if((first + count) > info->point_count) {
        res = LASER_ERROR_INVALID_RANGE;
    } else {
        stride = stride == 0 ? sizeof(laserPoint): stride;
        uint8_t* raw_point = ((uint8_t*) raw_points) + (first * info->point_size);
        laserPoint* point = points;
        uint64_t point_size = info->point_size;
        uint32_t point_format = info->point_format;
        float scale_x = info->scale_x;
        float scale_y = info->scale_y;
        float scale_z = info->scale_z;
        float offset_x = info->offset_x;
        float offset_y = info->offset_y;
        float offset_z = info->offset_z;
        for(uint64_t i = 0; i < count; i++) {
            point->x = ((float) *((int32_t*) (raw_point + _LASER_ATTRIB_OFFSET_TABLE[point_format][LASER_ATTRIB_TYPE_X])) * scale_x + offset_x);
            point->y = ((float) *((int32_t*) (raw_point + _LASER_ATTRIB_OFFSET_TABLE[point_format][LASER_ATTRIB_TYPE_Y])) * scale_y + offset_y);
            point->z = ((float) *((int32_t*) (raw_point + _LASER_ATTRIB_OFFSET_TABLE[point_format][LASER_ATTRIB_TYPE_Z])) * scale_z + offset_z);

            point->intensity = *((uint16_t*) (raw_point + _LASER_ATTRIB_OFFSET_TABLE[point_format][LASER_ATTRIB_TYPE_INTENSITY]));

            uint8_t flags = *((uint8_t*) (raw_point + _LASER_ATTRIB_OFFSET_TABLE[point_format][LASER_ATTRIB_TYPE_FLAG]));
            point->flags.return_number = flags & 0x07;
            point->flags.return_count = (flags >> 0x03) & 0x07;
            point->flags.scan_direction = (flags >> 0x06) & 0x01;
            point->flags.edge = (flags >> 0x07) & 0x01;

            uint8_t classification = *((uint8_t*) (raw_point + _LASER_ATTRIB_OFFSET_TABLE[point_format][LASER_ATTRIB_TYPE_CLASSIFICATION]));
            point->classification.type = classification & 0x07;
            point->classification.synthetic = (classification >> 0x05) & 0x01;
            point->classification.keypoint = (classification >> 0x06) & 0x01;
            point->classification.withheld = (classification >> 0x07) & 0x01;

            point->scan_angle = *((int8_t*) (raw_point + _LASER_ATTRIB_OFFSET_TABLE[point_format][LASER_ATTRIB_TYPE_SCAN_ANGLE]));
            point->usr = *((uint8_t*) (raw_point + _LASER_ATTRIB_OFFSET_TABLE[point_format][LASER_ATTRIB_TYPE_USR]));
            point->point_id = *((uint16_t*) (raw_point + _LASER_ATTRIB_OFFSET_TABLE[point_format][LASER_ATTRIB_TYPE_POINT_ID]));

            point = (laserPoint*) (((uint8_t*) point) + stride);
            raw_point += point_size;
        }
    }
    return res;
}

laserError laser_read_from_mem(laserPoint* points, uint64_t stride, void* mem, uint64_t size) {
    return laser_read_range_from_mem(points, stride, mem, size, 0, LASER_ALL_POINTS);
}

laserError laser_read_range_from_mem(laserPoint* points, uint64_t stride, void* mem, uint64_t size, uint64_t first, uint64_t count) {
    laserInfo info;
    laserError res = laser_info_from_mem(&info, mem, size);
    if(res == LASER_ERROR_NONE) {
        count = count == LASER_ALL_POINTS ? info.point_count: count;
        res = _laser_read_from_mem(points, stride, &info, ((uint8_t*) mem) + info.point_offset, size, first, count);
    }
    return res;
}

laserError laser_info_from_io(laserInfo* info, laserIo* io, void* usr) {
    laserError res = LASER_ERROR_NONE;
    laserPublicHeaderBlock public_header_block;
    uint32_t read = io->read(usr, (void*) &public_header_block, sizeof(laserPublicHeaderBlock), 0);
    if(read == sizeof(laserPublicHeaderBlock)) {
        res = laser_info_from_mem(info, (void*) &public_header_block, read);
    } else {
        res = LASER_ERROR_IO_READ;
    }
    return res;
}

laserError laser_read_from_io(laserPoint* points, uint64_t stride, laserIo* io, void* usr) {
    return laser_read_range_from_io(points, stride, io, usr, 0, LASER_ALL_POINTS);
}

laserError laser_read_range_from_io(laserPoint* points, uint64_t stride, laserIo* io, void* usr, uint64_t first, uint64_t count) {
    laserInfo info;
    laserError res = laser_info_from_io(&info, io, usr);
    if(res == LASER_ERROR_NONE) {
        uint8_t point_buffer[16384 * 4];
        LASER_MEMSET(point_buffer, 0, sizeof(point_buffer));

        count = count == LASER_ALL_POINTS ? info.point_count: count;
        uint64_t max_point_count = sizeof(point_buffer) / info.point_size;
        uint64_t point_count = count > max_point_count ? max_point_count: count;

        uint64_t read_count = count / max_point_count;
        read_count = read_count == 0 ? 1: read_count;

        uint64_t expected = info.point_size * point_count;
        for(uint64_t i = 0; i < read_count; i++) {
            uint64_t offset = info.point_offset + ((i + first) * info.point_size * point_count);
            uint32_t read = io->read(usr, (void*) point_buffer, expected, offset);
            if(read == expected) {
                res = _laser_read_from_mem(points + (i * point_count), stride, &info, (void*) point_buffer, read, 0, point_count);
                if(res != LASER_ERROR_NONE) {
                    break;
                }
            } else {
                res = LASER_ERROR_IO_READ;
                break;
            }
        }
    }
    return res;
}

const char* laser_error_str(laserError err) {
    switch(err) {
        case LASER_ERROR_NONE: return "No error";
        case LASER_ERROR_INVALID_FILE: return "Unknown file format";
        case LASER_ERROR_INVALID_RANGE: return "Invalid point range";
        case LASER_ERROR_VERSION_UNSUPPORTED: return "Unsupported version, supported versions: 1.0, 1.1, 1.2 and 1.3";
        case LASER_ERROR_FORMAT_UNSUPPORTED: return "Unknown point format, known formats: 0, 1, 2, 3, 4 and 5";
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
