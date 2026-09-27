#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <unistd.h>

static void make_directory(const char *path)
{
    if (mkdir(path, 0755) < 0 && errno != EEXIST)
        dprintf(STDERR_FILENO, "mkdir %s: %s\n", path, strerror(errno));
}

static void mount_if_needed(const char *source, const char *target,
                            const char *type, unsigned long flags,
                            const char *data)
{
    make_directory(target);

    if (mount(source, target, type, flags, data) < 0 && errno != EBUSY)
        dprintf(STDERR_FILENO, "mount %s on %s: %s\n",
                type, target, strerror(errno));
}

static void attach_console(void)
{
    int fd = open("/dev/console", O_RDWR | O_NOCTTY);

    if (fd < 0)
        return;

    if (fd != STDIN_FILENO)
        dup2(fd, STDIN_FILENO);
    if (fd != STDOUT_FILENO)
        dup2(fd, STDOUT_FILENO);
    if (fd != STDERR_FILENO)
        dup2(fd, STDERR_FILENO);
    if (fd > STDERR_FILENO)
        close(fd);
}

static void print_file(const char *label, const char *path)
{
    char buffer[4096];
    ssize_t count;
    int fd = open(path, O_RDONLY);

    if (fd < 0)
        return;

    dprintf(STDOUT_FILENO, "%s", label);

    while ((count = read(fd, buffer, sizeof buffer)) > 0)
        write(STDOUT_FILENO, buffer, (size_t) count);

    if (count == 0)
        write(STDOUT_FILENO, "\n", 1);

    close(fd);
}

static void print_nul_strings(const char *label, const char *path)
{
    char buffer[4096];
    ssize_t count;
    int fd = open(path, O_RDONLY);

    if (fd < 0)
        return;

    count = read(fd, buffer, sizeof buffer - 1);
    close(fd);

    if (count <= 0)
        return;

    buffer[count] = '\0';
    for (ssize_t i = 0; i < count; i++) {
        if (buffer[i] == '\0')
            buffer[i] = ' ';
    }

    dprintf(STDOUT_FILENO, "%s%s\n", label, buffer);
}

int main(void)
{
    struct utsname system_name;

    /*
     * Mount devtmpfs before reopening the console. CONFIG_DEVTMPFS_MOUNT may
     * already have mounted it; EBUSY is harmless in that case.
     */
    mount_if_needed("devtmpfs", "/dev", "devtmpfs", MS_NOSUID, "mode=0755");
    attach_console();

    write(STDOUT_FILENO,
          "\n=== Cheap Phone OS / MIRO A1 / stage 0 ===\n",
          sizeof("\n=== Cheap Phone OS / MIRO A1 / stage 0 ===\n") - 1);

    mount_if_needed("proc", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, NULL);
    mount_if_needed("sysfs", "/sys", "sysfs", MS_NOSUID | MS_NODEV | MS_NOEXEC, NULL);

    if (uname(&system_name) == 0) {
        dprintf(STDOUT_FILENO, "kernel: %s %s %s %s\n",
                system_name.sysname,
                system_name.release,
                system_name.version,
                system_name.machine);
    }

    print_file("cmdline: ", "/proc/cmdline");
    print_nul_strings("device-tree model: ",
                      "/sys/firmware/devicetree/base/model");
    print_nul_strings("device-tree compatible: ",
                      "/sys/firmware/devicetree/base/compatible");

    write(STDOUT_FILENO,
          "stage 0 init is alive; no Android userspace has been started.\n",
          sizeof("stage 0 init is alive; no Android userspace has been started.\n") - 1);

    for (;;)
        pause();
}
