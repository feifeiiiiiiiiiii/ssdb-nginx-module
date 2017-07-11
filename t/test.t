# vi:ft=

use lib 'lib';
use Test::Nginx::Socket 'no_plan';

repeat_each(4);

#master_on;
#worker_connections 1024;

#no_diff;
no_long_string;

#log_level 'warn';

run_tests();

__DATA__

=== TEST 1: raw ssdb proto
--- config
    location /raw_ssdb {
        set $key "3\nset\n3\nabc\n4\n1234\n\n";
        ssdb_raw_query $key;
        ssdb_pass 127.0.0.1:8888;
    }
--- request
    GET /raw_ssdb
--- response_body eval
"2\nok\n1\n1\n\n"
--- no_error_log
[error]


=== TEST 2: raw ssdb proto from query
--- config
    location /raw_ssdb_q {
        set_unescape_uri $q $arg_q;
        ssdb_raw_query $q;
        ssdb_pass 127.0.0.1:8888;
    }
--- request
    GET /raw_ssdb_q?q=3%0Aset%0A3%0Aabc%0A4%0A1234%0A%0A
--- response_body eval
"2\nok\n1\n1\n\n"
--- no_error_log
[error]

=== TEST 3: literal ssdb proto
--- config
    location /literal_ssdb {
        ssdb_literal_query "3\nset\n3\nabc\n4\n1234\n\n";
        ssdb_pass 127.0.0.1:8888;
    }
--- request
    GET /literal_ssdb
--- response_body eval
"2\nok\n1\n1\n\n"
--- no_error_log
[error]

