#!/bin/bash

# Create directories
sudo mkdir -p /home/DinoDB/socket
sudo mkdir -p /home/DinoDB/tmp
sudo mkdir -p /home/DinoDB/logs
sudo mkdir -p /home/DinoDB/installation_files
sudo mkdir -p /home/DinoDB/DinoDB\ Server\ 1.0
sudo mkdir -p /home/DinoDB/dinodb/ids
sudo mkdir -p /home/DinoDB/dinodb/DBS
sudo mkdir -p /home/DinoDB/dinodb/DBS/databases
sudo mkdir -p /home/DinoDB/dinodb/DBS/dbfiles
sudo mkdir -p /home/DinoDB/dinodb/DBS/tmp
sudo mkdir -p /home/DinoDB/dinodb/DBS/users
sudo mkdir -p /home/DinoDB/dinodb/ids/dbid
sudo mkdir -p /home/DinoDB/dinodb/ids/tableid
sudo mkdir -p /home/DinoDB/dinodb/ids/userid
	
# Create files
sudo touch /home/DinoDB/dinodb.pid
sudo touch /home/DinoDB/logs/dinodb.log
sudo touch /home/DinoDB/logs/dinodb-slow.log
 sudo touch /home/DinoDB/logs/dinodb-bin.log  # Commented out for replication
sudo touch /home/DinoDB/dinodb/DBS/dbfiles/DBS.DBS
 sudo touch /home/DinoDB/logs/dinodb-server.log

# Create the create_directory.sh file

sudo bash -c 'cat <<EOL > /home/DinoDB/installation_files/create_directory.sh
#!/bin/bash


directory_name=\$1


sudo mkdir -p /home/DinoDB/dinodb/DBS/databases/"\$directory_name"

if [ -d /home/DinoDB/dinodb/DBS/databases/"\$directory_name" ]; then
    echo ""
else
    echo "Something is wrong, please restart as Administrator."
fi
EOL'

# Make the script executable
sudo chmod +x /home/DinoDB/installation_files/create_directory.sh


# Remove the create_directory.sh file
sudo bash -c 'cat <<EOL > /home/DinoDB/installation_files/remove_directory.sh
#!/bin/bash

directory_name=\$1

sudo rm -rf /home/DinoDB/dinodb/DBS/databases/"\$directory_name"


if [  -d /home/DinoDB/dinodb/DBS/databases/"\$directory_name" ]; then
    echo "Something is wrong, please restart as Administrator."
else
    echo ""
fi

EOL'

# Make the script executable
sudo chmod +x /home/DinoDB/installation_files/remove_directory.sh

# Create a hidden system user and group
#sudo groupadd -r dinodbgroup
#sudo useradd -r -s /usr/sbin/nologin -g dinodbgroup dinodbuser

# Change ownership of the directories and files to dinodbuser
#sudo chown -R dinodbuser:dinodbgroup /home/DinoDB

# Set permissions: 
# root should have full control, and dinodbuser should have read and write access
#sudo chmod -R 700 /home/DinoDB
sudo chmod +x /home/DinoDB
sudo chmod +x /home/DinoDB/*
# Ensure that root has full control over the directories and files
#sudo chmod -R 755 /home/DinoDB

# Create dinodb.ini file and add content
sudo bash -c 'cat <<EOL > /home/DinoDB/DinoDB\ Server\ 1.0/dinodb.ini
# Common DinoDB Settings
[DinoDB]
user = DinoDB
port = 10757
socket = /home/DinoDB/socket/dinodb.sock
pid-file = /home/DinoDB/dinodb.pid
basedir = /home/DinoDB
tmpdir = /home/DinoDB/tmp

# DinoDB Server Settings
[client]
port = 10757
socket = /home/DinoDB/socket/dinodb.sock
default-character-set = utf8mb4
# Network Settings
bind-address = 127.0.0.1
max_connections = 200
max_allowed_packet = 64M

# Character Set and Collation
character-set-server = utf8mb4
collation-server = utf8mb4_unicode_ci

# Input and Output Logs
log-error = /home/DinoDB/logs/dinodb.log
slow_query_log = 1
slow_query_log_file = /home/DinoDB/logs/dinodb-slow.log
long_query_time = 2

# RAM Settings
key_buffer_size = 16M
max_heap_table_size = 32M
tmp_table_size = 32M
table_open_cache = 2000
thread_cache_size = 50
query_cache_type = 1
query_cache_size = 32M

# DinoDB Server Settings
[DinoDB]
no-auto-rehash
default-character-set = utf8mb4

# Engineering Tools
[isamchk]
key_buffer_size = 16M
sort_buffer_size = 16M

# Replication Settings
# [DinoDB]
# server-id = 1
# log_bin = /home/DinoDB/logs/dinodb-bin.log
# binlog_format = ROW
EOL'

echo "Configuration file created: /home/DinoDB/DinoDB Server 1.0/dinodb.ini"
sudo ./server
