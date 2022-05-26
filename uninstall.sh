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
soundpack_target="/usr/share/sounds/windowslonghorn"

# Greetings
_greetings() {
    echo -e "\n${BOLD}${BLUE}## Uninstalling Reza's DWM Build ##${RESET}"
    echo "This is a heavily patched DWM to help you building your own"
}

# Checking superuser program
# Checking whether using doas or sudo
# I personally use doas but since most people use sudo,
# I decided to catch that possibility
_superuserDoas() {
    checkDoas="$(type doas >/dev/null)"
    if [ -n "${checkDoas}" ]; then
        superuser="doas"
        return 0
    fi
    return 1
}
_superuserSudo() {
    checkSudo="$(type sudo >/dev/null)"
    if [ -n "${checkSudo}" ]; then
        superuser="sudo"
        return 0
    fi
    return 1
}
_checkingSuperuser() {
    echo -e "\n${BOLD}${BLUE}::${WHITE} Checking verification program${RESET}"
    if [ -n _superuserDoas ]; then
        superuser="doas"
        echo -e "'doas' is installed, ${GREEN}Proceeding...${RESET}"
        return 0
    elif [ -n _superuserSudo ]; then
        echo -e "'doas' is not installed, but you have 'sudo'"
        superuser="sudo"
        return 0
    else
        echo -e "${RED}\n You have no program to access as root${RESET}"
        echo -e "${BOLD}Please install 'doas' or 'sudo'${RESET}"
        exit 1
    fi
}

# Removing `config.h` to start fresh from `config.def.h`
_removeConfigH() {
    echo -e "\n${BOLD}${BLUE}::${WHITE} Removing 'config.h'${RESET}"
    if [ -e config.h ]; then
        echo -e "'config.h' exists, ${GREEN} Removing... ${RESET}"
        if ! (rm -f config.h); then
            echo -e "${RED} Unable to remove 'config.h' ${RESET}"
            return 1
        else
            echo -e "'config.h' removed"
            return 0
        fi
    else
        echo -e "'config.h' does not exist,${GREEN} Proceeding...${RESET}"
        return 0
    fi
}

# Checking dependency
_requiredDependencyCheck() {
    echo -e "\n${BOLD}${BLUE}::${WHITE} Checking required dependencies${RESET}"
    echo -e " ${RED}You can uninstall these program to clean up your system${RESET}"
    for program in $required_dependencies; do
        if ! (pacman -Q | grep "${program}" >/dev/null); then
            echo -e "${RED}'$program' is not installed${RESET}"
            exit 1
        else
            echo -e "'$program' is installed, "
        fi
    done
}
_optionalDependencyCheck() {
    echo -e "\n${BOLD}${BLUE}::${WHITE} Checking optional dependencies${RESET}"
    echo -e " ${RED}You can uninstall these program to clean up your system${RESET}"
    for program in $optional_dependencies; do
        if ! (pacman -Q | grep "${program}" >/dev/null); then
            echo -e "'$program' is not installed"
        else
            echo -e "'$program' is installed"
        fi
    done
}

# Copying 'dwm.desktop' to '/usr/share/xsessions'
_removeDesktop() {
    echo -e "\n${BOLD}${BLUE}::${WHITE} Removing 'dwm.desktop'${RESET}"
    if [ -e /usr/share/xsessions/dwm.desktop ]; then
        read -p "'dwm.desktop' file already exists, do you want to remove it? (Y/n)" confirmRemove
        case ${confirmRemove} in
            n | N) echo "No" && return 0 ;;
            y | Y | *)
                echo "Removing 'dwm.desktop'"
                ${superuser} rm /usr/share/xsessions/dwm.desktop
                echo -e "'dwm.desktop' removed, ${GREEN}Proceeding..."
                return 0
                ;;
        esac
    else
        echo -e "'dwm.desktop' already removed, ${GREEN}Proceeding..."
        return 0
    fi
}

_uninstallSoundpack() {
    echo -e "\n${BOLD}${BLUE}::${WHITE} Removing soundpack ${RESET}"
    if [ -e /usr/share/sounds/windowslonghorn ]; then
        ${superuser} rm -rf ${soundpack_target}
        echo -e "Soundpack removed, ${GREEN}Proceeding...${RESET}"
        return 0
    else
        echo -e "Soundpack already removed, ${GREEN}Proceeding...${RESET}"
        return 0
    fi
}

# Final Message
_installReport() {
    if [ $? -ne 0 ]; then
        echo -e "${RED}\n Installation unsuccessful\n Check the log above or install required dependencies\n ${RESET}"
        exit 1
    else
        echo -e "${BOLD}${GREEN}\n ## Congratulations, Reza's DWM Build is removed from your system! ##\n ${RESET}"
        return 0
    fi
}

# 'dwm' installation
_dwmUninstallation() {
    echo -e "\n${BOLD}${BLUE}::${WHITE} Uninstalling 'dwm' with '${superuser}'${RESET}"
    if ! ${superuser} make clean uninstall; then
        _installReport
        exit 1
    else
        _optionalDependencyCheck
        _installReport
        exit 0
    fi
}

### Main execution
_greetings
_requiredDependencyCheck
_checkingSuperuser
_removeConfigH
_removeDesktop
_uninstallSoundpack
_dwmUninstallation
