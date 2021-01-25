Contents
========
* Version Generation Tool
    * Git Version Generation Tool
    * Svn Version Generation Tool

Git Version Generation Tool
----------------
Generate version file before build project when using git as version control tool
*   Requirements
    *   Install GIT package (default path: GIT_INSTALL_DIR="C:\Program Files")
    *   Environment:  Add "GIT_INSTALL_DIR\bin" to the system path

*   Add script and version template in Keil
    *  Copy GitGenerateVersion.sh and git_version.template to the path of the project
    *  Open project by Keil
        *   Options for targets... => User tab
        *   Select Run#1 below "Before Build/Rebuild"
        *   Add User Command:
                bash.exe GitGenerateVersion.sh [Tag_Header]
        *   Stop on Exit Code:  Select ">=1"

    [Tag_Header]:   Header of the tag. The format of git tag must be "Tag_headerMAJOR.MINOR.REVISION" (ex. "bee2-v0.9.1"). And the script will
                    generate the build version: "MAJOR.MINOR.REVISION.COMMIT_COUNT" (ex. 0.9.1.4535).  Tag_header must be added in the command
                    line as a parameter to help the script to remove the header from tag. It can be ommited if the tag header is "bee2-v".

*   Build Error
    *   Eror Message:    "Error: CreatePross failed, Command: 'bash.exe GitGenerateVersion.sh'"
    *   Should add the "GIT_INSTALL_DIR\bin" to your system path
    *   GIT_INSTALL_DIR is the git installation path

SVN Version Generation Tool
----------------
Generate version file before build project when using svn as version control tool
*   Requirements
    *   Install TortoiseSVN package (default path: SVN_INSTALL_DIR=C:\Program Files)
    *   Environment:  Add "SVN_INSTALL_DIR\TortoiseSVN\bin" to the system path

*   Add script and version template in Keil
    *   Copy SvnGenerateVersion.sh and svn_version.template to the path of the project
    *   Open project by Keil
        *   Options for targets... => User tab
        *   Select Run#1 below "Before Build/Rebuild"
        *   Add User Command:
                cmd.exe /c SvnGenerateVersion.bat

*   Note:   Version format      MAJOR.MINOR.REVISION.BUILDDATE
            SvnGenerateVersion.bat update the REVISION and BUILDDATE field of the template.  If want to change the MAJOR(or MINOR) of the version, 
            you need to modify the svn_version.template before generate the version file.
