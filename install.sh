#!/usr/bin/env bash

# Text Manipulation
GREEN="\e[92m"
RED="\e[91m"
BLUE="\e[94m"
WHITE="\e[39m"
BOLD="\e[1m"
RESET="\e[0m"

# Superuser programs
# You can define yours here 'sudo' or 'doas'
# or let the script check for you
superuser=""

# Dependencies
required_dependencies="yajl polybar rofi rofimoji playerctl scrot lightdm powertop nitrogen picom dunst unclutter pipewire redshift parcellite xfce4-power-manager polkit-gnome unclutter syncthing-gtk blueman"
optional_dependencies="alacritty zotero librewolf firefox nemo calibre zoom networkmanager yad mpv transmission-gtk"

# Greetings
_greetings () {
    echo -e "\n${BOLD}${BLUE}## Installing Reza's DWM Build ##${RESET}"
    echo "This is a heavily patched DWM to help you building your own"
}

# Checking superuser program
# Checking whether using doas or sudo
# I personally use doas but since most people use sudo,
# I decided to catch that possibility
_superuserDoas () {
    checkDoas="$(type doas > /dev/null)"
    if [ -n "${checkDoas}" ]; then
        superuser="doas"
        return 0;
    fi
    return 1;
}
_superuserSudo () {
    checkSudo="$(type sudo > /dev/null)"
    if [ -n "${checkSudo}" ]; then
        superuser="sudo"
        return 0;
    fi
    return 1;
}
_checkingSuperuser () {
    echo -e "\n${BOLD}${BLUE}::${WHITE} Checking verification program${RESET}"
    if [ -n _superuserDoas ]; then
        superuser="doas"
        echo -e "'doas' is installed, ${GREEN}Proceeding...${RESET}"
        return 0;
    elif [ -n _superuserSudo ]; then
        echo -e "'doas' is not installed, but you have 'sudo'"
        superuser="sudo"
        return 0;
    else
        echo -e "${RED}\n You have no program to access as root${RESET}"
        echo -e "${BOLD}Please install 'doas' or 'sudo'${RESET}"
        exit 1;
    fi
}

# Removing `config.h` to start fresh from `config.def.h`
_removeConfigH () {
    echo -e "\n${BOLD}${BLUE}::${WHITE} Removing 'config.h'${RESET}"
    if [ -e config.h ]; then
        echo -e "'config.h' exists, ${GREEN} Removing... ${RESET}"
        if ! (rm -f config.h); then
            echo -e "${RED} Unable to remove 'config.h' ${RESET}"
            return 1;
        else
            echo -e "'config.h' removed"
            return 0;
        fi
    else
        echo -e "'config.h' does not exist,${GREEN} Proceeding...${RESET}"
        return 0;
    fi
}

# Checking dependency
_requiredDependencyCheck () {
    echo -e "\n${BOLD}${BLUE}::${WHITE} Checking required dependencies${RESET}"
    for program in $required_dependencies
    do
        if ! (pacman -Q | grep "${program}" > /dev/null); then
            echo -e "${RED}'$program' is not installed${WHITE}\n Please install it first with <pacman -S ${program}>${RESET}"
            exit 1;
        else
            echo -e "'$program' is installed, ${GREEN}Proceeding...${RESET}"
        fi
    done
}
_optionalDependencyCheck () {
    echo -e "\n${BOLD}${BLUE}::${WHITE} Checking optional dependencies\n You may install these programs to enhance your experience${RESET}"
    for program in $optional_dependencies
    do
        if ! (pacman -Q | grep "${program}" > /dev/null); then
            echo -e "'$program' is not installed"
        else
            echo -e "'$program' is installed"
        fi
    done
}

# Copying 'dwm.desktop' to '/usr/share/xsessions'
_confirmDesktopCopy () {
    if [ -e /usr/share/xsessions/dwm.desktop ]; then
        read -p "'dwm.desktop' file already exists, do you want to replace it? (y/n)" confirmReplace
        case ${confirmReplace} in
            y|Y ) echo "Replacing 'dwm.desktop'"
                  rm /usr/share/xsessions/dwm.desktop && ${superuser} cp ./dwm.desktop /usr/share/xsessions
                  echo "'dwm.desktop' replaced"
                  return 0;;
            n|N ) echo "No" && return 0;;
            * ) return 0;;
        esac
    else
        ${superuser} cp ./dwm.desktop /usr/share/xsessions
    fi
}
_copyDesktop () {
	echo -e "\n${BOLD}${BLUE}::${WHITE} Copying '.desktop' file ${RESET}"
	echo "(Do this if you use Display Manager [LightDM,SDDM,GDM,etc.] )"
	echo "Remember that this DWM build is engineered to interacts with LightDM"
	read -p "Do you want to copy '.desktop' file to '/usr/share/xsessions'? (y/n)" cpDesktop
	case ${cpDesktop} in
		y|Y ) echo "yes" && _confirmDesktopCopy;;
		n|N ) echo "no"; return 0;;
		* ) return 0;;
    esac
}

# Final Message
_installReport () {
    if [ $? -ne 0 ]; then
        echo -e "${RED}\n Installation unsuccessful\n Check the log above or install required dependencies\n ${RESET}"
        exit 1;
    else
        echo -e "${BOLD}${GREEN}\n ## Congratulations, Reza's DWM Build is at your disposal! ##\n ${RESET}"
        return 0;
    fi
}

# 'dwm' installation
_dwmInstallation () {
    echo -e "\n${BOLD}${BLUE}::${WHITE} Installing 'dwm' with '${superuser}'${RESET}"
    if ! ${superuser} make clean install; then
        _installReport
        exit 1;
    else
        _optionalDependencyCheck
        _installReport
        exit 0;
    fi
}

### Main execution
_greetings
_requiredDependencyCheck
_checkingSuperuser
_removeConfigH
_copyDesktop
_dwmInstallation
