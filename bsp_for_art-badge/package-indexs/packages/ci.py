# -*- coding:utf-8 -*-

import sys
import os
import stat
import time
import datetime
import subprocess
import shlex
import shutil
import json
import requests


def execute_command(cmdstring, cwd=None, shell=True):
    """Execute the system command at the specified address."""

    if shell:
        cmdstring_list = cmdstring

    sub = subprocess.Popen(cmdstring_list, cwd=cwd, stdin=subprocess.PIPE,
                           stdout=subprocess.PIPE, shell=shell, bufsize=4096)

    stdout_str = ''
    while sub.poll() is None:
        stdout_str += sub.stdout.read()
        time.sleep(0.1)

    return stdout_str


def determine_url_valid(url_from_srv):
    """Check the validity of urls."""

    # check url is github
    if not url_from_srv.startswith("https://github.com"):
        print("not support url: {}".format(url_from_srv))
        return False

    headers = {'Connection': 'keep-alive',
               'Accept-Encoding': 'gzip, deflate',
               'Accept': '*/*',
               'User-Agent': 'curl/7.54.0'}

    try:
        for i in range(0, 3):
            r = requests.get(url_from_srv, stream=True, headers=headers)
            if r.status_code == requests.codes.not_found:
                if i == 2:
                    print("Warning : %s is invalid." % url_from_srv)
                    return False
                time.sleep(1)
            else:
                break

        return True

    except Exception as e:
        print('Error message:%s\t' % e)
        print('Network connection error or the url : %s is invalid.\n' %
              url_from_srv)


def get_json_info(json_pathname):
    with open(json_pathname, 'r+') as f:
        json_content = f.read()

    try:
        package_info = json.loads(json_content)
    except ValueError:
        print('The JSON config file syntax checking failed!')
        return False

    return package_info


def file_path_check(package_info, pathname):
    info_dir = os.path.dirname(pathname)

    if package_info['name'] == os.path.basename(info_dir):
        return True
    else:
        print("===========================================>")
        print("Error: package name is different with package folder name.")
        print(pathname)
        print("package name:%s" % package_info['name'])
        print("package folder name: %s" % os.path.basename(info_dir))
        return False


def check_json_file(work_root):
    """Check the json file."""

    file_count = 1
    folder_walk_result = os.walk(work_root)

    for path, d, file_list in folder_walk_result:
        for filename in file_list:
            if filename == 'package.json':
                json_pathname = os.path.join(path, 'package.json')
                json_info = get_json_info(json_pathname)
                print("\nNo.%d" % file_count)
                file_count += 1
                if not json_file_content_check(json_info):
                    return False

                if not file_path_check(json_info, json_pathname):
                    return False

    return True


def json_file_content_check(package_info):
    """Check the content of json file."""

    if package_info['category'] == '':
        print('The category of ' + package_info['name'] + ' package is lost.')
        return False

    if 'enable' not in package_info or package_info['enable'] == '':
        print('The enable of ' + package_info['name'] + ' package is lost.')
        return False

    if package_info['author']['name'] == '':
        print('The author name of ' + package_info['name'] + ' package is lost.')
        return False

    if package_info['author']['email'] == '':
        print('The author email of ' + package_info['name'] + ' package is lost.')
        return False

    if package_info['license'] == '':
        print('The license of ' + package_info['name'] + ' package is lost.')
        return False

    if package_info['repository'] == '':
        print('The repository of ' + package_info['name'] + ' package is lost.')
        return False
    else:
        if not determine_url_valid(package_info['repository']):
            return False

    for i in range(0, len(package_info['site'])):
        package_version = package_info['site'][i]['version']
        package_url = package_info['site'][i]['URL']
        print("%s : %s" % (package_version, package_url))
        if not package_url[-4:] == '.git':
            print(package_info['site'][i]['filename'])
        if not determine_url_valid(package_url):
            return False

    return True


def main():
    """The entry point of the script."""

    try:
        work_root = os.getcwd()
        print(work_root)
        if not check_json_file(work_root):
            sys.exit(1)

        sys.exit(0)

    except Exception as e:
        print('Error message:%s\t' % e)
        sys.exit(1)


if __name__ == '__main__':
    main()
