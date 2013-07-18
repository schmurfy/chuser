#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// strerror
#include <string.h>
#include <errno.h>

// getpwnam
#include <sys/types.h>
#include <pwd.h>
// #include <uuid/uuid.h>


// getgrnam
#include <grp.h>

// int setgid(gid_t gid);
// int setuid(uid_t uid);

#define err(...) { fprintf(stderr, __VA_ARGS__); exit(1); }

int main(int argc, char const *argv[])
{
  const char *account, *user, *group, *cmd;
  struct passwd *pw;
  struct group *grp;
  char **args;
  int len, n = 0;
  
  if( argc < 4)
    err("Version: 1.0\nUsage: %s <user> <group> <cmd>\n", argv[0]);
    
  account = *++argv;
  group = *++argv;
  cmd = *++argv;
  
  args = malloc(argc - 2); // cmd + args + NULL
  
  // copy args
  do {
    len = strlen(*argv);
    args[n] = malloc(len + 1);
    strncpy(args[n], *argv, len + 1); // size + '\0'
    n += 1;
  } while(*++argv != NULL);
  
  args[n] = NULL;
  
  
  pw = getpwnam(account);
  if( !pw )
    err("Unknown user: %s\n", account);
  
  if( strcmp(group, "_") ){
    grp = getgrnam(group);
    if( !grp )
      err("Unknown group: %s\n", group);
    
    
    if( setgid(grp->gr_gid) == -1 )
      err("Cannot change gid: %s\n", strerror(errno));
  }
  
  if( setuid(pw->pw_uid) == -1 )
    err("Cannot change uid: %s\n", strerror(errno));

  printf("cmd: %s\n", cmd);
  execv(cmd, args);
  
  err("exec failed: %s\n", strerror(errno));
  
  return 0;
}
