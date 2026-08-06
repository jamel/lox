#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

#define MAX_PROMPT_LEN 255
#define MAX_FILE_LEN 1048576LL

#define ATTR_PRINTF(start_index, first_to_check) \
  __attribute__((format(printf, start_index, first_to_check)))

static void log_errv(int rv, const char* fmt, va_list va) {
  fprintf(stderr, "lox: ");
  vfprintf(stderr, fmt, va);
  fprintf(stderr, ": %s (%d)\n", strerror(rv), rv);
}

ATTR_PRINTF(2, 3)
static void log_errf(int rv, const char* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  log_errv(rv, fmt, va);
  va_end(va);
}

ATTR_PRINTF(1, 2)
static void log_errno(const char* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  log_errv(errno, fmt, va);
  va_end(va);
}

int read_file(const char* filename, char** buf, size_t* len) {
  FILE* fp = fopen(filename, "rb");
  if (fp == 0) {
    log_errno("cannot open file \'%s\'", filename);
    return ENOENT;
  }

  int rv = fseek(fp, 0, SEEK_END);
  if (rv) {
    log_errf(rv, "failed to get file \'%s\' size", filename);
    fclose(fp);
    return EINVAL;
  }

  long file_size = ftell(fp);
  if (file_size < 0) {
    fclose(fp);
    log_errno("failed to get file \'%s\' size", filename);
    return EINVAL;
  }

  if (file_size > MAX_FILE_LEN) {
    fclose(fp);
    log_errf(E2BIG, "file \'%s\' is too big", filename);
    return E2BIG;
  }

  rewind(fp);

  char* tmpbuf = malloc(file_size + 1);
  if (tmpbuf == NULL) {
    fclose(fp);
    log_errf(ENOMEM, "failed to allocate %ld bytes", file_size);
    return ENOMEM;
  }

  size_t read_bytes = fread(tmpbuf, 1, file_size, fp);
  if (read_bytes < (size_t)file_size) {
    free(tmpbuf);
    fclose(fp);
    log_errf(ENODATA, "failed to read data from file \'%s\'", filename);
    return ENODATA;
  }

  tmpbuf[read_bytes] = '\0';
  fclose(fp);

  *buf = tmpbuf;
  *len = file_size;
  return 0;
}

int run_line(const char* line) {
  fprintf(stderr, "run: \'%s\'\n", line);
  return 0;
}

int run_file(const char* filename) {
  char* buf;
  size_t size;
  int rv = read_file(filename, &buf, &size);
  if (rv) {
    log_errf(rv, "filed to read file \'%s\'", filename);
    return rv;
  }

  char* start = buf;
  char* end;
  while ((end = strchr(start, '\n')) != NULL) {
    if (end != start) {
      *end = '\0';

      rv = run_line(start);
      if (rv) {
        log_errf(rv, "failed to run line \'%s'", start);
        free(buf);
        return EINVAL;
      }
    }

    start = end + 1;
  }

  free(buf);
  return 0;
}

int run_prompt() {
  char line[MAX_PROMPT_LEN];
  int rv;
  size_t len;

  puts("Lox v0.1");
  puts("Press Ctlr-D to exit\n");

  for (;;) {
    printf(">> ");
    if (fgets(line, MAX_PROMPT_LEN, stdin) != NULL) {
      len = strlen(line);
      if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len--;
      }
      if (len == 0) {
        continue;
      }

      rv = run_line(line);
      if (rv) {
        log_errf(rv, "failed to run line \'%s\'", line);
        return rv;
      }
    } else {
      break;
    }
  }

  return 0;
}

int main(int argc, const char** argv) {
  if (argc > 2) {
    fprintf(stderr, "Usage: %s [script]\n", argv[0]);
    return EXIT_FAILURE;
  } else if (argc == 2) {
    return run_file(argv[1]);
  } else {
    return run_prompt();
  }
}
