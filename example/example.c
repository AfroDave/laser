#include <stdlib.h>
#include <stdio.h>

#define LASER_IMPL
#include "../laser.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/stat.h>
#endif

typedef struct v3 {
    float x, y, z;
} v3;

uint64_t las_read(void* usr, void* data, uint64_t size, uint64_t offset);

uint64_t las_read(void* usr, void* data, uint64_t size, uint64_t offset) {
    FILE* f = (FILE*) usr;
    fseek(f, offset, SEEK_SET);
    return fread(data, 1, size, f);
}

int main(int argc, const char** argv) {
    if(argc > 1) {
        unsigned long file_size = 0;
#if defined(_WIN32)
        HANDLE h = CreateFileA(argv[1], GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        DWORD s = 0;
        GetFileSize(h, &s);
        file_size = s;
#else
        struct stat s;
        stat(argv[1], &s);
        file_size = s.st_size;
#endif

        FILE* file = fopen(argv[1], "rb");

        void* las_data = malloc(file_size);
        fread(las_data, 1, file_size, file);
        laserInfo info;
        laser_info_from_mem(&info, las_data, file_size);

        laserPoint* points = (laserPoint*) calloc(info.point_count, sizeof(laserPoint));
        laser_read_from_mem(points, 0, las_data, file_size);

        laserAttrib attribs[] = {
            { LASER_ATTRIB_TYPE_X, 0 },
            { LASER_ATTRIB_TYPE_Y, 4 },
            { LASER_ATTRIB_TYPE_Z, 8 },
            LASER_ATTRIB_END
        };

        v3* xyz = (v3*) calloc(info.point_count, sizeof(laserPoint));
        laser_read_range_from_mem_with_attribs(xyz, sizeof(v3), attribs, las_data, file_size, 0, LASER_ALL_POINTS);

        laser_read_from_io(points, 0, &las_read, (void*) file);

        for(uint32_t i = 0; i < 1000; i++) {
            printf("%d %.2f %.2f %.2f\n",
                    points[i].flags.return_number,
                    points[i].x / (info.max_x - info.min_x),
                    points[i].y / (info.max_y - info.min_y),
                    points[i].z / (info.max_z - info.min_z));
        }
        free(las_data);
        free(points);
        fclose(file);
        return 0;
    }
    printf("%s FILE\n", argv[0]);
    return -1;
}
