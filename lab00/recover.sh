#/bin/bash

USER_PATH="user"
BIN_PATH="bin"
USER_LIST_PATH="config/users.list"
USER_LIST_BACKUP_PATH="config/users.list.bak"

# Find the intruder
intruders=()
for intruder in $(diff -c $USER_LIST_BACKUP_PATH $USER_LIST_PATH | grep "^\+"); do
    if [ $intruder != '+' ]
    then
        intruders+=(${intruder})
    fi
#    echo ${intruder}
done
echo "Intruders: ${intruders[@]}"

# Find modified programs
malware=()
for bin in $(ls $BIN_PATH); do
    if [ $BIN_PATH/$bin -nt $USER_LIST_PATH ]
    then
        malware+=($bin)
    fi
done
echo "Programs may be edited by the intruder: ${malware[@]}"

# Find affected users
files=()
affectedUser=()
for user in $(ls $USER_PATH); do
    for app in $(ls $USER_PATH/$user/init.d); do
        if grep -Pq "(sudo|dog)" $USER_PATH/$user/init.d/$app
        then
            affectedUser+=($user)
            files+=($USER_PATH/$user/init.d/$app)
        fi
    done
done
echo "Users may be affected: ${affectedUser[@]}"

# Remove all compromised lines
pattern=$(printf "|%s" ${malware[@]})
pattern="(${pattern:1})"
for f in "${files[@]}"; do
    sed -Ei "/$pattern/d" $f
done

# Recover users.list
rm config/users.list && mv config/users.list.bak config/users.list
