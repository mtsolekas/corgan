#include "corgan_paths.h"
#include "corgan_schedule.h"

int init_schedule()
{
    FILE *fp;

    if (access(SCHEDULE_PATH, F_OK | R_OK | W_OK)) {
        fp = fopen(SCHEDULE_PATH, "w");
        if (!fp) return -1;

        fprintf(fp, "%s", "\0");

        fclose(fp);
    }

    if (read_schedule_file()) return -1;

    return 0;
}

int free_schedule()
{
    free(sched);

    return 0;
}

int read_schedule_file()
{
    FILE *fp;
    char c;
    int size, i;

    fp = fopen(SCHEDULE_PATH, "r");
    if (!fp) return -1;

    size = 100;
    sched = malloc(sizeof(char) * size);
    if (!sched) return -1;

    for (i = 0; (c = getc(fp)) != EOF; ++i) {
        if (i >= size) {
            size += 100;
            sched = realloc(sched, sizeof(char) * size);
            if (!sched) return -1;
        }

        sched[i] = c;
    }

    fclose(fp);

    sched[i] = '\0';
    sched = realloc(sched, sizeof(char) * (++i));
    if (!sched) return -1;

    return 0;
}

int write_schedule_file()
{
    FILE *fp;

    fp = fopen(SCHEDULE_PATH, "w");
    if (!fp) return -1;

    fputs(sched, fp);
    fclose(fp);

    return 0;
}
