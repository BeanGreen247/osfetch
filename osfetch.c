#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>

int main() {
    // Get OS information
    FILE *fp = fopen("/etc/os-release", "r");
    char line[256];
    char *os_pretty_name = NULL;
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
            os_pretty_name = strdup(line + 12);
            os_pretty_name[strlen(os_pretty_name) - 1] = '\0'; // Remove the trailing newline
            break;
        }
    }
    fclose(fp);

    // Get kernel version
    FILE *proc_version = fopen("/proc/version", "r");
    char kernel_version[256];
    fgets(kernel_version, sizeof(kernel_version), proc_version);
    fclose(proc_version);

    // Extract the kernel version
    char *kernel_version_start = strstr(kernel_version, "version ") + 8;
    char *kernel_version_end = strchr(kernel_version_start, ' ');
    *kernel_version_end = '\0';

    // Get system uptime
    struct sysinfo info;
    sysinfo(&info);
    int uptime_days = info.uptime / (24 * 60 * 60);
    int uptime_hours = (info.uptime / (60 * 60)) % 24;
    int uptime_minutes = (info.uptime / 60) % 60;
    int uptime_seconds = info.uptime % 60;

    // Get CPU information
    FILE *proc_cpuinfo = fopen("/proc/cpuinfo", "r");
    char cpu_name[256];
    int cpu_cores = 0;
    while (fgets(line, sizeof(line), proc_cpuinfo)) {
        if (strncmp(line, "model name", 10) == 0) {
            char *cpu_name_start = strchr(line, ':') + 2;
            strcpy(cpu_name, cpu_name_start);
            cpu_name[strlen(cpu_name) - 1] = '\0'; // Remove the trailing newline
        } else if (strncmp(line, "processor", 9) == 0) {
            cpu_cores++;
        }
    }
    fclose(proc_cpuinfo);

    // Get GPU information
    FILE *proc_pci = popen("lspci -v | grep VGA", "r");
    char gpu_name[256];
    fgets(gpu_name, sizeof(gpu_name), proc_pci);
    char *gpu_name_start = strchr(gpu_name, ':') + 2;
    gpu_name[strlen(gpu_name_start) - 1] = '\0'; // Remove the trailing newline
    pclose(proc_pci);

    // Get memory information
    struct sysinfo mem_info;
    sysinfo(&mem_info);
    int total_memory = mem_info.totalram / (1024 * 1024);
    int free_memory = mem_info.freeram / (1024 * 1024);
    int available_memory = mem_info.freeram / (1024 * 1024);

    // Print the results
    printf("Os: %s\n", os_pretty_name);
    printf("Kernel: %s\n", kernel_version_start);
    printf("Uptime: Days:%d Hours:%02d Minutes:%02d Seconds:%02d\n", uptime_days, uptime_hours, uptime_minutes, uptime_seconds);
    printf("CPU: %s (%d)\n", cpu_name, cpu_cores);
    printf("GPU: %s\n", gpu_name_start);
    printf("Memory: %d MiB/%d MiB\n", total_memory - available_memory, total_memory);
    printf("Available Memory: %d MiB\n", available_memory);

    free(os_pretty_name);
    return 0;
}
