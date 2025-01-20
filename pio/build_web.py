from subprocess import check_output, CalledProcessError
import os
import platform

Import("env")

def is_tool(name):
    """Check if a tool exists in PATH."""
    cmd = "where" if platform.system() == "Windows" else "which"
    try:
        check_output([cmd, name])
        return True
    except:
        return False

def build_web():
    if is_tool("npm"):
        project_dir = os.path.dirname(env["PROJECT_DIR"])
        source_dir = os.path.join(project_dir, "PulSar", "code", "6_Lights", "03_Animator", "source")

        print("Changing to source directory:", source_dir)
        os.chdir(source_dir)  # Change to source directory for npm build

        print("Attempting to build webpage...")
        try:
            # Run npm install (ensure dependencies are up to date)
            check_output(["npm.cmd", "install", "--only=dev"], shell=True)

            # Run the actual build process
            print(check_output(["npm.cmd", "run", "build"], shell=True).decode("utf-8"))

            # Verify the build file exists
            build_output = os.path.join(source_dir, "build", "index.html.gz.h")
            print(f"Looking for built file at: {build_output}")

            if not os.path.exists(build_output):
                raise FileNotFoundError(f"Built file not found: {build_output}")

            print("Build process completed successfully.")
        except OSError as e:
            print(f"Encountered OSError while building webpage: {e}")
        except CalledProcessError as e:
            print(f"Encountered CalledProcessError: {e.output.decode('utf-8')}")
        except Exception as e:
            print(f"Unexpected error ({type(e).__name__}): {e}")
        finally:
            # Reset directory to project root
            os.chdir(project_dir)
            print("Returning to project directory.")
    else:
        print("npm is not available. Skipping web build.")

build_web()



# from shutil import copyfile
# from subprocess import check_output, CalledProcessError
# import sys
# import os
# import platform
# import subprocess

# Import("env")

# def is_tool(name):
#     cmd = "where" if platform.system() == "Windows" else "which"
#     try:
#         check_output([cmd, name])
#         return True
#     except:
#         return False

# def build_web():
#     if is_tool("npm"):
#         # os.chdir("U:\GitHub\PulSar")
#         os.chdir("U:/GitHub/PulSar/code/6_Lights/03_Animator/source")
#         print("Attempting to build webpage...")
#         try:
#             if platform.system() == "Windows":
#                 print(check_output(["npm run install", "install", "--only=dev"]))
#                 # print(check_output(["npm.cmd", "install", "--only=dev"]))
#                 # print(check_output(["node_modules\\.bin\\gulp.cmd"]))
#             # else:
#             #     print(check_output(["npm", "install"]))
#             #     print(check_output(["node_modules/.bin/gulp"]))
#             # copyfile("build/index.html.gz.h", "../dist/index.html.gz.h")
#         except OSError as e:
#             print("Encountered error OSError building webpage:", e)
#             if e.filename:
#                 print("Filename is", e.filename)
#             print("WARNING: Failed to build web package. Using pre-built page.")
#         except CalledProcessError as e:
#             print(e.output)
#             print("Encountered error CalledProcessError building webpage:", e)
#             print("WARNING: Failed to build web package. Using pre-built page.")
#         except Exception as e:
#             print("Encountered error", type(e).__name__, "building webpage:", e)
#             print("WARNING: Failed to build web package. Using pre-built page.")
#         finally:
#             os.chdir("..");

# build_web()