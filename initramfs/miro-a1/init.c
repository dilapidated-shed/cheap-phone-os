/*
 * Cheap Phone OS — MIRO A1 stage-0 init
 *
 * Freestanding by design: no libc, Android runtime, or dynamic linker.
 * Syscall numbers below are Linux ARM EABI numbers.
 */

typedef unsigned int usize;

#define NR_EXIT   1
#define NR_READ   3
#define NR_WRITE  4
#define NR_OPEN   5
#define NR_CLOSE  6
#define NR_MOUNT  21
#define NR_PAUSE  29
#define NR_MKDIR  39
#define NR_DUP2   63
#define NR_UNAME  122

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define O_RDWR   00000002
#define O_NOCTTY 00000400

#define MS_NOSUID 2
#define MS_NODEV  4
#define MS_NOEXEC 8

struct uts_name {
    char sysname[65];
    char nodename[65];
    char release[65];
    char version[65];
    char machine[65];
    char domainname[65];
};

static long call0(long number)
{
    register long r0 __asm__("r0");
    register long r7 __asm__("r7") = number;

    __asm__ volatile("svc 0"
                     : "=r"(r0)
                     : "r"(r7)
                     : "memory", "cc");
    return r0;
}

static long call1(long number, long a0)
{
    register long r0 __asm__("r0") = a0;
    register long r7 __asm__("r7") = number;

    __asm__ volatile("svc 0"
                     : "+r"(r0)
                     : "r"(r7)
                     : "memory", "cc");
    return r0;
}

static long call2(long number, long a0, long a1)
{
    register long r0 __asm__("r0") = a0;
    register long r1 __asm__("r1") = a1;
    register long r7 __asm__("r7") = number;

    __asm__ volatile("svc 0"
                     : "+r"(r0)
                     : "r"(r1), "r"(r7)
                     : "memory", "cc");
    return r0;
}

static long call3(long number, long a0, long a1, long a2)
{
    register long r0 __asm__("r0") = a0;
    register long r1 __asm__("r1") = a1;
    register long r2 __asm__("r2") = a2;
    register long r7 __asm__("r7") = number;

    __asm__ volatile("svc 0"
                     : "+r"(r0)
                     : "r"(r1), "r"(r2), "r"(r7)
                     : "memory", "cc");
    return r0;
}

static long call5(long number, long a0, long a1, long a2, long a3, long a4)
{
    register long r0 __asm__("r0") = a0;
    register long r1 __asm__("r1") = a1;
    register long r2 __asm__("r2") = a2;
    register long r3 __asm__("r3") = a3;
    register long r4 __asm__("r4") = a4;
    register long r7 __asm__("r7") = number;

    __asm__ volatile("svc 0"
                     : "+r"(r0)
                     : "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r7)
                     : "memory", "cc");
    return r0;
}

static usize text_length(const char *text)
{
    usize count = 0;

    while (text[count] != '\0')
        count++;

    return count;
}

static void write_text(int fd, const char *text)
{
    usize left = text_length(text);
    const char *cursor = text;

    while (left != 0) {
        long wrote = call3(NR_WRITE, fd, (long)cursor, left);

        if (wrote <= 0)
            return;

        cursor += wrote;
        left -= (usize)wrote;
    }
}

static void make_directory(const char *path)
{
    (void)call2(NR_MKDIR, (long)path, 0755);
}

static void mount_tree(const char *source, const char *target,
                       const char *type, unsigned long flags,
                       const char *data)
{
    make_directory(target);
    (void)call5(NR_MOUNT,
                (long)source,
                (long)target,
                (long)type,
                (long)flags,
                (long)data);
}

static void attach_console(void)
{
    long fd = call3(NR_OPEN,
                    (long)"/dev/console",
                    O_RDWR | O_NOCTTY,
                    0);

    if (fd < 0)
        return;

    if (fd != STDIN_FILENO)
        (void)call2(NR_DUP2, fd, STDIN_FILENO);
    if (fd != STDOUT_FILENO)
        (void)call2(NR_DUP2, fd, STDOUT_FILENO);
    if (fd != STDERR_FILENO)
        (void)call2(NR_DUP2, fd, STDERR_FILENO);

    if (fd > STDERR_FILENO)
        (void)call1(NR_CLOSE, fd);
}

static void print_file(const char *label, const char *path)
{
    char buffer[1024];
    long fd = call3(NR_OPEN, (long)path, 0, 0);

    if (fd < 0)
        return;

    write_text(STDOUT_FILENO, label);

    for (;;) {
        long count = call3(NR_READ, fd, (long)buffer, sizeof buffer);

        if (count <= 0)
            break;

        (void)call3(NR_WRITE, STDOUT_FILENO, (long)buffer, count);
    }

    write_text(STDOUT_FILENO, "\n");
    (void)call1(NR_CLOSE, fd);
}

static void print_nul_strings(const char *label, const char *path)
{
    char buffer[1024];
    long fd = call3(NR_OPEN, (long)path, 0, 0);
    long count;
    long index;

    if (fd < 0)
        return;

    count = call3(NR_READ, fd, (long)buffer, sizeof buffer - 1);
    (void)call1(NR_CLOSE, fd);

    if (count <= 0)
        return;

    for (index = 0; index < count; index++) {
        if (buffer[index] == '\0')
            buffer[index] = ' ';
    }
    buffer[count] = '\0';

    write_text(STDOUT_FILENO, label);
    write_text(STDOUT_FILENO, buffer);
    write_text(STDOUT_FILENO, "\n");
}

static void print_uname(void)
{
    struct uts_name system_name;

    if (call1(NR_UNAME, (long)&system_name) < 0)
        return;

    write_text(STDOUT_FILENO, "kernel: ");
    write_text(STDOUT_FILENO, system_name.sysname);
    write_text(STDOUT_FILENO, " ");
    write_text(STDOUT_FILENO, system_name.release);
    write_text(STDOUT_FILENO, " ");
    write_text(STDOUT_FILENO, system_name.version);
    write_text(STDOUT_FILENO, " ");
    write_text(STDOUT_FILENO, system_name.machine);
    write_text(STDOUT_FILENO, "\n");
}

static void stage0_main(void)
{
    mount_tree("devtmpfs", "/dev", "devtmpfs", MS_NOSUID, "mode=0755");
    attach_console();

    write_text(STDOUT_FILENO,
               "\n=== Cheap Phone OS / MIRO A1 / stage 0 ===\n");

    mount_tree("proc",
               "/proc",
               "proc",
               MS_NOSUID | MS_NODEV | MS_NOEXEC,
               (const char *)0);
    mount_tree("sysfs",
               "/sys",
               "sysfs",
               MS_NOSUID | MS_NODEV | MS_NOEXEC,
               (const char *)0);

    print_uname();
    print_file("cmdline: ", "/proc/cmdline");
    print_nul_strings("device-tree model: ",
                      "/sys/firmware/devicetree/base/model");
    print_nul_strings("device-tree compatible: ",
                      "/sys/firmware/devicetree/base/compatible");

    write_text(STDOUT_FILENO,
               "stage 0 init is alive; no Android userspace has been started.\n");

    for (;;)
        (void)call0(NR_PAUSE);
}

__attribute__((noreturn)) void _start(void)
{
    stage0_main();
    (void)call1(NR_EXIT, 0);

    for (;;) {
    }
}
