/*
 * Copyright (c) 2008, 2018, Oracle and/or its affiliates. All rights reserved.
 *               2020, 2021 MariaDB Corporation AB
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2.0, as
 * published by the Free Software Foundation.
 *
 * This program is also distributed with certain software (including
 * but not limited to OpenSSL) that is licensed under separate terms,
 * as designated in a particular file or component or in included license
 * documentation.  The authors of MySQL hereby grant you an
 * additional permission to link the program and your derivative works
 * with the separately licensed software that they have included with
 * MySQL.
 *
 * Without limiting anything contained in the foregoing, this file,
 * which is part of MySQL Connector/C++, is also subject to the
 * Universal FOSS Exception, version 1.0, a copy of which can be found at
 * http://oss.oracle.com/licenses/universal-foss-exception.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License, version 2.0, for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
 */


#include "ConnCpp.h"

#include "tests_config.h"

#include <string>
#include <iostream>
#include <system_error>

using namespace std;

int loops = 2;

static sql::Driver * driver = nullptr;

/* {{{	*/
static sql::Connection *
get_connection(const std::string & host, const std::string & user, const std::string & pass, bool useTls=TEST_USETLS)
{
  try {
    /* There will be concurrency problem if we had threads, but don't have, then it's ok */
    if (!driver) {
      driver = sql::mariadb::get_driver_instance();
    }
    if (loops % 2 && !useTls) {
      return driver->connect(host, user, pass);
    } else {
      sql::ConnectOptionsMap connection_properties;
      connection_properties["hostName"]= host;
      connection_properties["userName"]= user;
      connection_properties["password"]= pass;
      connection_properties["useTls"]=   useTls ? "true" : "false";
      /* We need to run tests for client- and server-side prepared statements. That also gives
       much more sense for these tests to be run twice */
      connection_properties["useServerPrepStmts"] = "true";

      return driver->connect(connection_properties);
    }
  } catch (sql::SQLException &e) {
    cout << "sql::SQLException caught during connect" << endl;
    cout << e.what() << endl;
    throw;
  }
}
/* }}} */

#define DRIVER_TEST 1
#define TEST_COMMON_TAP_NAME "driver_test"

#include "test_common.cpp"

static void driver_test_new_driver_exception()
{
  // We do not export Driver interface implementation
  /*try {
    new sql::mariadb::MariaDbDriver();
    ensure("Exception not thrown", false);
  } catch (sql::InvalidArgumentException&) { }*/
}

/* {{{	*/
int main(int argc, const char **argv)
{
  driver_test_new_driver_exception();

  return run_tests(argc, argv);
}
/* }}} */

