# Reverse Proxy


./app -t postgres --local-host 127.0.0.1 --local-port 7777 --forward-host 127.0.0.1 --forward-port 5432

./sysbench --db-driver=pgsql --oltp-table-size=100000 --oltp-tables-count=240 --threads=100 --pgsql-host=127.0.0.1 --pgsql-port=7777 --pgsql-user=sbtest --pgsql-password=password --pgsql-db=test /usr/share/sysbench/tests/include/oltp_legacy/parallel_prepare.lua cleanup
./sysbench --db-driver=pgsql --oltp-table-size=100000 --oltp-tables-count=240 --threads=100 --pgsql-host=127.0.0.1 --pgsql-port=7777 --pgsql-user=sbtest --pgsql-password=password --pgsql-db=test /usr/share/sysbench/tests/include/oltp_legacy/parallel_prepare.lua prepare

