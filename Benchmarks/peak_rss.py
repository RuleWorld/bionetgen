import os
import subprocess
import sys
import resource

def main():
    if len(sys.argv) < 2:
        sys.exit(1)

    pid = os.fork()
    if pid == 0:
        # Child
        try:
            os.execvp(sys.argv[1], sys.argv[1:])
        except Exception as e:
            print(f"Error executing {sys.argv[1]}: {e}", file=sys.stderr)
            os._exit(1)
    else:
        # Parent
        _, exit_status, rusage = os.wait4(pid, 0)
        # rusage.ru_maxrss is in KB on Linux
        print(f"PEAK_RSS_KB: {rusage.ru_maxrss}", file=sys.stderr)
        if os.WIFEXITED(exit_status):
            sys.exit(os.WEXITSTATUS(exit_status))
        else:
            sys.exit(1)

if __name__ == "__main__":
    main()
