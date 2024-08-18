#include "../include/minishell.h"

void    ft_built_exit(t_args *args, t_mini *mini)
{
    (void)mini;
    (void)args;
}

void    ft_built_unset(t_args *args, t_mini *mini)
{
    int i;
    if (args->argc == 1)        //MIRAR
        return ;
    i = 1;
    while (args->args[i])
    {
        ft_unset_env(args->args[i], mini);
        i++;
    }
}

void    ft_built_export(t_args *args, t_mini *mini)
{
    int i;

    if (args->argc == 1)
    {
        mini->env_iter = mini->env_first_node;
	    while (mini->env_iter != NULL)
	    {
	    	printf("declare -x %s=\"%s\"\n", mini->env_iter->variable, mini->env_iter->content);
    		mini->env_iter = mini->env_iter->next;
    	}
        return ;
    }
    i = 1;
    while (args->args[i])
    {
        if (ft_strchr(args->args[i], '='))
            ft_export_env(args->args[i], mini);
        i++;
    }
}

void    ft_built_pwd(t_args *args)
{
    char    *temp;
    char    *buffer;
    int     i;
    int     j;

    i = 1;
    if (args->argc != 1)
    {
        if (args->args[1][0] == '-')
        {
            printf("-bash: pwd: %s: invalid option\n", args->args[1]);
            return ;
        }
        while (args->args[i])
        {
            j = 0;
            while (args->args[i][j])
            {
             if (ft_strchr("<>|", args->args[i][j])) //MIRAR
                 return ;
             j++;
         }
         i++;
        }
    }
    buffer = NULL;
    temp = getcwd(buffer, INT_MAX);
    if (!temp)
        return ;
    printf("%s\n", temp);
}

void    ft_built_cd(t_args *args, t_mini *mini)
{
    char    *output;
    char    *buffer;
    char    *temp;

    buffer = NULL;
    ft_export_env(ft_strjoin("OLDPWD=", ft_get_env("PWD", mini)), mini);
    output = getcwd(buffer,  INT_MAX);
    //if (!output)
    temp = ft_strjoin(output, "/");
    free(output);
    free(buffer);
    output = ft_strjoin(temp, args->args[1]);
    free(temp);
    chdir(output);
    output = getcwd(buffer, INT_MAX);
    //if (!output)
    ft_export_env(ft_strjoin("PWD=", output), mini);
    free(output);
}

void    ft_built_echo(t_args *args, int flag)
{
    int i;
    int j;

    i = 1;
    while (args->args[i])
    {
        j = 0;
        while (args->args[i][j])
        {
            if (ft_strchr("<>|", args->args[i][j])) //MIRAR
                return ;
            j++;
        }
        i++;
    }
    i = 1;
    if (flag == 1)
        i = 2;
    while (args->args[i])
    {
        printf("%s", args->args[i]);
        if (i != args->argc - 1)
            printf(" ");
        i++;
    }
    if (flag == 0)
        printf("\n");
}

int ft_built_ins(t_args *args, t_mini *mini)
{
    int flag;

    flag = 0;
    if (args->argc > 1 && ft_strncmp(args->args[0], "echo", ft_strlen(args->args[0])) == 0)
    {
        if (ft_strncmp(args->args[1], "-n", ft_strlen(args->args[1])) == 0)
            flag = 1;
        ft_built_echo(args, flag);
    }
    if (args->argc == 2 && ft_strncmp(args->args[0], "cd", ft_strlen(args->args[0])) == 0)
        ft_built_cd(args, mini);
    if (ft_strncmp(args->args[0], "pwd", ft_strlen(args->args[0])) == 0)
        ft_built_pwd(args);
    if (ft_strncmp(args->args[0], "export", ft_strlen(args->args[0])) == 0)
        ft_built_export(args, mini);
    if (ft_strncmp(args->args[0], "unset", ft_strlen(args->args[0])) == 0)
        ft_built_unset(args, mini);
    if (args->argc == 1 && ft_strncmp(args->args[0], "env", ft_strlen(args->args[0])) == 0)
        ft_print_env(mini);
    if (args->argc == 1 && ft_strncmp(args->args[0], "exit", ft_strlen(args->args[0])) == 0)
        ft_built_exit(args, mini);
    return (0);
}

/*
echo funciona si solo pones echo sin redirecciones ni pipes ni nada
cd funciona
pwd funciona si solo pones pwd sin redirecciones ni pipes ni nada
export funciona
unset funciona
env funciona
exit hay que hacerlo
*/

/*
◦ echo con la opción -n.
◦ cd solo con una ruta relativa o absoluta. (no entre pipes)
◦ pwd sin opciones.
◦ export sin opciones. (no entre pipes)
◦ unset sin opciones. (no entre pipes)
◦ env sin opciones o argumentos.
◦ exit sin opciones. (no entre pipes)


ines@FS26QF2:~/minishell_1$ pwd
/home/ines/minishell_1
ines@FS26QF2:~/minishell_1$ pwd k
/home/ines/minishell_1
ines@FS26QF2:~/minishell_1$ pwd cat
/home/ines/minishell_1
ines@FS26QF2:~/minishell_1$ pwd cat eaw
/home/ines/minishell_1
ines@FS26QF2:~/minishell_1$ echo hola && pwd
hola
/home/ines/minishell_1
ines@FS26QF2:~/minishell_1$ echo hola | pwd
/home/ines/minishell_1
ines@FS26QF2:~/minishell_1$ pwd cada,makfjs,m´wpiugh  eqklwbsdl.s < wakdl vmenqwl
bash: wakdl: No such file or directory
ines@FS26QF2:~/minishell_1$ pwd cada,makfjs,m´wpiugh  eqklwbsdl.s  wakdl vmenqwl
/home/ines/minishell_1

ines@FS26QF2:~/minishell_1$ cd src/ src/ include/
bash: cd: too many arguments

export ines=iness hola=casa
ines@FS26QF2:~/minishell_1$ echo $ines $hola
iness casa
ines@FS26QF2:~/minishell_1$ export iii
ines@FS26QF2:~/minishell_1$ echo $iii (iii no existe)

ines@FS26QF2:~/minishell_1$ export iii= (iii sí existe)
ines@FS26QF2:~/minishell_1$ echo $iii

ines@FS26QF2:~/minishell_1$ export iii=ines
ines@FS26QF2:~/minishell_1$ echo $iii
ines
ines@FS26QF2:~/minishell_1$ export
declare -x DBUS_SESSION_BUS_ADDRESS="unix:path=/run/user/1000/bus"
declare -x DISPLAY=":0"
declare -x HOME="/home/ines"
declare -x HOSTTYPE="x86_64"
declare -x LANG="C.UTF-8"
declare -x LESSCLOSE="/usr/bin/lesspipe %s %s"
declare -x LESSOPEN="| /usr/bin/lesspipe %s"
declare -x LOGNAME="ines"
declare -x LS_COLORS="rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.webp=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:"
declare -x MOTD_SHOWN="update-motd"
declare -x NAME="FS26QF2"
declare -x OLDPWD="/home/ines"
declare -x PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/usr/lib/wsl/lib:/mnt/c/Program Files/WindowsApps/CanonicalGroupLimited.Ubuntu_2204.3.49.0_x64__79rhkp1fndgsc:/mnt/c/WINDOWS/system32:/mnt/c/WINDOWS:/mnt/c/WINDOWS/System32/Wbem:/mnt/c/WINDOWS/System32/WindowsPowerShell/v1.0/:/mnt/c/WINDOWS/System32/OpenSSH/:/mnt/c/Program Files/dotnet/:/mnt/c/Program Files/nodejs/:/mnt/c/Program Files/Docker/Docker/resources/bin:/mnt/c/Users/FS26QF2/AppData/Local/Microsoft/WindowsApps:/mnt/c/Users/FS26QF2/Documents/notebook/Microsoft VS Code/bin:/mnt/c/Users/FS26QF2/AppData/Local/Programs/Microsoft VS Code Insiders/bin:/mnt/c/Users/FS26QF2/AppData/Roaming/npm:/snap/bin"
declare -x PULSE_SERVER="unix:/mnt/wslg/PulseServer"
declare -x PWD="/home/ines/minishell_1"
declare -x SHELL="/bin/bash"
declare -x SHLVL="3"
declare -x TERM="xterm-256color"
declare -x USER="ines"
declare -x WAYLAND_DISPLAY="wayland-0"
declare -x WSL2_GUI_APPS_ENABLED="1"
declare -x WSLENV=""
declare -x WSL_DISTRO_NAME="Ubuntu"
declare -x WSL_INTEROP="/run/WSL/3952_interop"
declare -x XDG_DATA_DIRS="/usr/local/share:/usr/share:/var/lib/snapd/desktop"
declare -x XDG_RUNTIME_DIR="/run/user/1000/"

*/
