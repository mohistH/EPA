#include "SqliteExt.h"
#include <QDebug>

namespace oct_sqlite
{
	/// --------------------------------------------------------------------------------
	/// @brief: oct_sqlite::SqliteExt::table_data_map_
	/// --------------------------------------------------------------------------------
	template<class TKey, class TValue>
	int oct_sqlite::SqliteExt::table_data_map_(const QString str_table_name, std::map<TKey, TValue>* pout_map, const int ui_key_column_index) noexcept
	{
		/// 1. 参数第二个无效，
		if (nullptr == pout_map)
		{
			return 1;
		}

		/// 2. 无法找到参数中的key， 
		QSqlTableModel* pmodel = table_model_(str_table_name);
		if (nullptr == pmodel)
		{
			return 2;
		}

		mapTableColumn map_column_type = table_column_type_(pmodel, str_table_name);

		/// ---------------------------------------------------------------------------------------
		/// 3. 主键也有效， 直接读取数据
		/// ---------------------------------------------------------------------------------------
		/// 得到当前表列数
		const int column_total_count = map_column_type.count();
		/// 检查参数中的主键索引
		if ((0 > ui_key_column_index) || (ui_key_column_index > column_total_count))
		{
			return 3;
		}


		/// ---------------------------------------------------------------------------------------
		/// 将数据读入pdata中: 逐行读取， 逐行的同时，读取逐列
		int row_total_count = tableRowCount(pmodel, str_table_name);

		QSqlQuery sql_query = pmodel->query();
		sql_query.first();


		try
		{
			/// 读取行
			for (int row_index = 0; row_index < row_total_count; ++row_index)
			{
				/// ---------------------------------------------------------------------------------------
				/// 这里开始插入map
				TValue map_item_value;
				TKey* pkey = nullptr;
				TKey map_item_key = 0;

				void* pdata = (void*)&map_item_value;

				/// 读取列
				for (int column_index = 0; column_index < column_total_count; ++column_index)
				{
					/// 检查当前行是否为key
					if (column_index == ui_key_column_index)
					{
						pkey = (TKey*)pdata;
					}

					QString str_cur_column_type = map_column_type.value(column_index).str_type_;

					/// 当前类型为空，则跳过当前行
					if (true == str_cur_column_type.isEmpty())
					{
						break;
					}

					/// 如果当前的数据类型不在给定的解析列表中，则pass
					auto find_current_type_is_exist = map_analysis_db_type_.find(str_cur_column_type);
					/// 不在给定的类型中， 则pass
					if (find_current_type_is_exist == map_analysis_db_type_.end())
					{
						break;
					}

					/// 根据对应的数据类型做转换
					if (find_current_type_is_exist->second.type_trans_func_)
					{
						pdata = (this->*find_current_type_is_exist->second.type_trans_func_)(pdata, sql_query.value(column_index), true);
					}
					else
					{
						;/// 数据类型处理函数无效
					}
				}


				if (nullptr != pkey)
				{
					map_item_key = *pkey;

					///  一行读取结束， 将其插入map
					pout_map->emplace(map_item_key, map_item_value);
				}


				/// 读取下一行, 无法读取，则pass
				if (false == sql_query.next())
				{
					break;
				}
			}

		}
		catch (...)
		{

		}

		return 0;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: oct_sqlite::SqliteExt::update_table_map_
	/// --------------------------------------------------------------------------------
	template<typename TKey, typename TValue>
	int oct_sqlite::SqliteExt::updateTableData(const QString str_table_name, const std::map<TKey, TValue>* pmap_data, const uint key_column_index) noexcept
	{

		/// 1. 检查 参数 pmap_data
		if (nullptr == pmap_data)
		{
			return 1;
		}

		/// 2, 获取对应表格的model
		QSqlQueryModel* pmodel = table_model_(str_table_name);
		if (nullptr == pmodel)
		{
			return 2;
		}


		/// 3. 将数据写入数据库, 逐个单元格写入
		/// UPDATE 表名称 SET 列名称 = 新值 WHERE 列名称 = 某值
		/// UPDATE Person SET FirstName = 'Fred' WHERE LastName = 'Wilson' 
		/// UPDATE Person SET Address = 'Zhongshan 23', City = 'Nanjing'
		///							WHERE LastName = 'Wilson'

		/// 5. 获取表的列
		mapTableColumn map_table_column_name = table_column_type_(pmodel, str_table_name);

		/// 6. 检查列
		const uint table_column_total_count = map_table_column_name.count();
		if ((0 == table_column_total_count) || (key_column_index > table_column_total_count))
		{
			return 3;
		}

		/// 删除表格数据
		removeTableAllRows(pmodel, str_table_name);


		/// 9. 将数据写入数据库
		//std::map<TKey, TValue>::iterator find_value_it = pmap_data->begin();

		try
		{
			/// 一共有这么多行的数据需要写入
			for (auto find_value_it = pmap_data->begin(); find_value_it != pmap_data->end(); ++find_value_it)
			{


				QString str_update_sql = "UPDATE " + str_table_name + " SET ";
				/// 主键列名
				QString str_key_name = map_table_column_name.value(key_column_index).str_name_;
				QString str_key_type = map_table_column_name.value(key_column_index).str_type_;
				QString str_where_value;/// = where_value(str_key_name, pkey_value);

										/// 是否读取下一行
				bool is_read_next_line = false;

				/// 统计set 列数
				int set_index = 0;

				/// 列数据的起始地址
				void* pdata = (void*)&find_value_it->second;

				/// 每一行有这么多列的数据需要写入
				for (int column_index = 0; column_index < table_column_total_count; ++column_index)
				{
					QString str_value;
					/// 不能更新主键列
					if (key_column_index == column_index)
					{
						bool is_ok = false;
						pdata = column_sql_str_(str_key_type, pdata, &str_where_value, is_ok, false);
						if (false == is_ok)
						{
							return 7;
						}
						continue;
					}

					/// 第 X列的名称: id， name, value
					QString str_column_type = map_table_column_name.value(column_index).str_type_;
					QString str_column_name = map_table_column_name.value(column_index).str_name_;

					/// 拼接sql: set column=value
					bool is_ok = false;
					pdata = column_sql_str_(str_column_type, pdata, &str_value, is_ok, true);
					if (false == is_ok)
					{
						return 8;
					}

					/// 无法解析
					if (true == str_value.isEmpty())
					{
						is_read_next_line = true;
						break;
					}

					/// 如果这是第二次执行， 则需要添加 逗号
					if (0 < set_index)
					{
						str_update_sql += ", " + str_column_name + "=" + str_value;
						++set_index;
					}
					else
					{
						str_update_sql += str_column_name + "=" + str_value;
						++set_index;
					}
				}	/// end 总列数



					/// 如果不是读取下一行
				if (false == is_read_next_line)
				{
					/// 构建完整的sql语句
					str_update_sql += " WHERE " + str_key_name + "=" + str_where_value + ";";

					//qDebug() << "sql_str=" << str_update_sql;

					///  执行sql语句
					QSqlQuery sql_query = pmodel->query();
					sql_query.first();

					bool prepare_ret = sql_query.prepare(str_update_sql);

					/// 语法错误
					if (false == prepare_ret)
					{
						;
					}
					else
					{
						bool exec_ret = sql_query.exec();
						//qDebug() << "exec_ret=" << exec_ret;
					}
				}/// 读取下一行


				 /// 上面的一行已经写入完成， 下面开始写入下一行数据
			}	/// 有这么多行数据
		}
		catch (...)
		{
			;
		}

		return 0;
	}


	template<typename TKey, typename TValue>
	int oct_sqlite::SqliteExt::writeTableData(const QString str_table_name, const QMap<TKey, TValue>* pmap_data, const uint key_column_index) noexcept
	{
		/// 1. 检查 参数 pmap_data
		if (nullptr == pmap_data)
		{
			return 1;
		}

		/// 2, 获取对应表格的model
		QSqlQueryModel* pmodel = table_model_(str_table_name);
		if (nullptr == pmodel)
		{
			return 2;
		}

		/// 5. 获取表的列
		mapTableColumn map_table_column_name = table_column_type_(pmodel, str_table_name);

		/// 6. 检查列
		const uint table_column_total_count = map_table_column_name.count();
		if ((0 == table_column_total_count) || (key_column_index > table_column_total_count))
		{
			return 3;
		}

		/// ---------------------------------------------------------------------------------------
		/// 删除表格数据
		removeTableAllRows(pmodel, str_table_name);

		/// 执行多行插入
		auto execute_multi_row_insert = [&pmodel](const QString& sql_str)
		{
			///  执行sql语句
			QSqlQuery sql_query = pmodel->query();
			sql_query.first();
			bool prepare_ret = sql_query.prepare(sql_str);

			/// 语法错误
			if (false == prepare_ret)
			{
				//qDebug() << "1111111==== prepare_ret=" << prepare_ret;
			}
			else
			{
				bool exec_ret = sql_query.exec();
				//qDebug() << "2222222==== exec_ret=" << exec_ret;
			}
		};


		/// 9. 将数据写入数据库
		try
		{
			QString insert_prefix = "INSERT INTO " + str_table_name + "(";
			{
				int column_index = 0;
				/// 构造插入语句前缀
				for (const auto& item : map_table_column_name)
				{
					insert_prefix += " " + item.str_name_;
					++column_index;
					if (column_index != map_table_column_name.count())
					{
						insert_prefix += ", ";
					}
					else
					{
						insert_prefix += ") VALUES ";
					}
				}
			}

			const int max_row_write_count = 1000;
			int write_row_index = 0;
			/// 一共有这么多行的数据需要写入
			QString multi_line_insert;

			bool rest_data_to_write = false;
			for (auto find_value_it = pmap_data->begin(); find_value_it != pmap_data->end(); ++find_value_it)
			{
				QString single_row_value = "(";
				/// 列数据的起始地址
				void* pdata = (void*)&find_value_it.value();

				QString str_key_name = map_table_column_name.value(key_column_index).str_name_;
				//QString str_key_type = map_table_column_name.value(key_column_index).str_type_;

				/// 每一行有这么多列的数据需要写入
				for (int column_index = 0; column_index < table_column_total_count; ++column_index)
				{
					QString str_value;

					/// 第 X列的名称: id， name, value
					QString str_column_type = map_table_column_name.value(column_index).str_type_;
					QString str_column_name = map_table_column_name.value(column_index).str_name_;

					/// 拼接sql: set column=value
					bool is_ok = false;
					pdata = column_sql_str_(str_column_type, pdata, &str_value, is_ok, true);
					if (false == is_ok)
					{
						//return 8;
						break;
					}

					/// 无法解析
					if (true == str_value.isEmpty())
					{
						//continue;
						break;
					}

					/// 
					//INSERT INTO TABLE_NAME(column1, column2, column3, ...columnN)]
					//VALUES(value1, value2, value3, ...valueN);
					single_row_value += str_value;

					/// 如果这是最后一行， 则不添加逗号
					if ((column_index + 1) == table_column_total_count)
					{
						single_row_value += ")";
					}
					else
					{
						single_row_value += ", ";
					}
				}	/// end 总列数

				/// 统计当前写到了第几行
				++write_row_index;
				///  insert into deloment values('雄安命2',1,10,6),('雄安命3',1,10,6 )

				/// 如果当前已经达到了1000行， 则将数据写入文件
				if (0 == (write_row_index % max_row_write_count))
				{
					multi_line_insert = single_row_value;

					/// 构建多行插入语句
					multi_line_insert = insert_prefix + multi_line_insert;

					//qDebug() << "str_sqlite_insert=" << multi_line_insert;

					execute_multi_row_insert(multi_line_insert);
				}
				/// 继续找下一行数据
				else
				{
					if (write_row_index == pmap_data->count())
					{
						rest_data_to_write = true;
					}
					else
					{
						single_row_value += ", ";
					}

					multi_line_insert += single_row_value;
				}
			}	/// 有这么多行数据

			if (true == rest_data_to_write)
			{
				/// 构建多行插入语句
				multi_line_insert = insert_prefix + multi_line_insert;
				//qDebug() << "str_sqlite_insert=" << multi_line_insert;
				execute_multi_row_insert(multi_line_insert);
			}
		}
		catch (...)
		{
			;
		}

		return 0;
	}

	template<class TKey, class TValue>
	int oct_sqlite::SqliteExt::table_data_qmap_(const QString str_table_name, QMap<TKey, TValue>* pout_map, const int ui_key_column_index) noexcept
	{
		/// 1. 参数第二个无效，
		if (nullptr == pout_map)
		{
			return 1;
		}

		/// 2. 无法找到参数中的key， 
		QSqlTableModel* pmodel = table_model_(str_table_name);
		if (nullptr == pmodel)
		{
			return 2;
		}

		mapTableColumn map_column_type = table_column_type_(pmodel, str_table_name);

		/// ---------------------------------------------------------------------------------------
		/// 3. 主键也有效， 直接读取数据
		/// ---------------------------------------------------------------------------------------
		/// 得到当前表列数
		const int column_total_count = map_column_type.count();
		/// 检查参数中的主键索引
		if ((0 > ui_key_column_index) || (ui_key_column_index > column_total_count))
		{
			return 3;
		}


		/// ---------------------------------------------------------------------------------------
		/// 将数据读入pdata中: 逐行读取， 逐行的同时，读取逐列
		int row_total_count = tableRowCount(pmodel, str_table_name);

		QSqlQuery sql_query = pmodel->query();
		sql_query.first();


		try
		{
			/// 读取行
			for (int row_index = 0; row_index < row_total_count; ++row_index)
			{
				/// ---------------------------------------------------------------------------------------
				/// 这里开始插入map
				TValue map_item_value;
				TKey* pkey = nullptr;
				TKey map_item_key = 0;

				void* pdata = (void*)&map_item_value;

				/// 读取列
				for (int column_index = 0; column_index < column_total_count; ++column_index)
				{
					/// 检查当前行是否为key
					if (column_index == ui_key_column_index)
					{
						pkey = (TKey*)pdata;
					}

					QString str_cur_column_type = map_column_type.value(column_index).str_type_;

					/// 当前类型为空，则跳过当前行
					if (true == str_cur_column_type.isEmpty())
					{
						break;
					}

					/// 如果当前的数据类型不在给定的解析列表中，则pass
					auto find_current_type_is_exist = map_analysis_db_type_.find(str_cur_column_type);
					/// 不在给定的类型中， 则pass
					if (find_current_type_is_exist == map_analysis_db_type_.end())
					{
						break;
					}

					/// 根据对应的数据类型做转换
					if (find_current_type_is_exist->second.type_trans_func_)
					{
						pdata = (this->*find_current_type_is_exist->second.type_trans_func_)(pdata, sql_query.value(column_index), true);
					}
					else
					{
						;/// 数据类型处理函数无效
					}
				}


				if (nullptr != pkey)
				{
					map_item_key = *pkey;

					///  一行读取结束， 将其插入map
					pout_map->insert(map_item_key, map_item_value);
				}


				/// 读取下一行, 无法读取，则pass
				if (false == sql_query.next())
				{
					break;
				}
			}
		}
		catch (...)
		{

		}

		return 0;
	}




	/// --------------------------------------------------------------------------------
	/// @brief: oct_sqlite::SqliteExt::table_data_map_some_columns_
	/// --------------------------------------------------------------------------------
	template<class TKey, class TValue>
	int oct_sqlite::SqliteExt::table_data_map_some_columns_(const QString str_table_name, std::map<TKey, TValue>* pout_map, const int ui_key_column_index, const MapUIntUInt& map_read_column_index)noexcept
	{


		/// 1. 参数第二个无效，
		if (nullptr == pout_map)
		{
			return 1;
		}

		/// 2. 无法找到参数中的key， 
		QSqlTableModel* pmodel = table_model_(str_table_name);
		if (nullptr == pmodel)
		{
			return 2;
		}

		mapTableColumn map_column_type = table_column_type_(pmodel, str_table_name);

		/// ---------------------------------------------------------------------------------------
		/// 3. 主键也有效， 直接读取数据
		/// ---------------------------------------------------------------------------------------
		/// 得到当前表列数
		const int column_total_count = map_column_type.count();
		/// 检查参数中的主键索引
		if ((0 > ui_key_column_index) || (ui_key_column_index > column_total_count))
		{
			return 3;
		}

		/// 检查列的最大数是否在表格中
		{
			for (auto item : map_read_column_index)
			{
				/// 如果参数中传递的列大于当前表格的最大值，则pass
				if (item.first > (uint)column_total_count)
				{
					return 3;
				}
			}
		}

		/// ---------------------------------------------------------------------------------------
		/// 将数据读入pdata中: 逐行读取， 逐行的同时，读取逐列
		int row_total_count = tableRowCount(pmodel, str_table_name);

		QSqlQuery sql_query = pmodel->query();
		sql_query.first();



		try
		{
			/// 读取行
			for (int row_index = 0; row_index < row_total_count; ++row_index)
			{
				/// ---------------------------------------------------------------------------------------
				/// 这里开始插入map
				TValue map_item_value;
				TKey map_item_key = 0;

				void* pdata = (void*)&map_item_value;

				/// 读取列
				for (int column_index = 0; column_index < column_total_count; ++column_index)
				{
					/// 检查当前行是否为key
					/// 主键列， 献给主键赋值
					void* pread_data = nullptr;
					/// 当前列是否为主键列
					bool current_column_is_key = false;
					if (column_index == ui_key_column_index)
					{
						pread_data = (void*)&map_item_key;
						current_column_is_key = true;
					}
					/// 不是主键， 读取数据列
					else
					{
						/// 如果当前列是参数map指定需要读取的列， 再读取
						auto find_read_column_index_it = map_read_column_index.find((uint)column_index);

						/// 不是当前需要读取的列， 则继续找下一列
						if (find_read_column_index_it == map_read_column_index.end())
						{
							continue;
						}

						pread_data = pdata;
					}





					QString str_cur_column_type = map_column_type.value(column_index).str_type_;

					/// 当前类型为空，则跳过当前行
					if (true == str_cur_column_type.isEmpty())
					{
						break;
					}

					/// 如果当前的数据类型不在给定的解析列表中，则pass
					auto find_current_type_is_exist = map_analysis_db_type_.find(str_cur_column_type);
					/// 不在给定的类型中， 则pass
					if (find_current_type_is_exist == map_analysis_db_type_.end())
					{
						break;
					}

					/// 根据对应的数据类型做转换
					if (find_current_type_is_exist->second.type_trans_func_)
					{
						pread_data = (this->*find_current_type_is_exist->second.type_trans_func_)(pread_data, sql_query.value(column_index), !current_column_is_key);
					}
					else
					{
						;/// 数据类型处理函数无效
					}

					/// 如果是主键，则将结果赋值给主键
					if (current_column_is_key)
					{
						map_item_key = *((TKey*)pread_data);
					}
					/// 不是主键， 则将数据保存
					else
					{
						pdata = pread_data;
					}
				}

				///  一行读取结束， 将其插入map
				pout_map->emplace(map_item_key, map_item_value);


				/// 读取下一行, 无法读取，则pass
				if (false == sql_query.next())
				{
					break;
				}
			}
		}
		catch (...)
		{

		}


		return 0;
	}



	/// --------------------------------------------------------------------------------
	/// @brief: oct_sqlite::SqliteExt::table_data_map_except_columns_
	/// --------------------------------------------------------------------------------
	template<class TKey, class TValue>
	int oct_sqlite::SqliteExt::table_data_map_except_columns_(const QString str_table_name, std::map<TKey, TValue>* pout_map, const int ui_key_column_index, const MapUIntUInt& map_read_except_column_index) noexcept
	{

		/// 1. 参数第二个无效，
		if (nullptr == pout_map)
		{
			return 1;
		}

		/// 2. 无法找到参数中的key， 
		QSqlTableModel* pmodel = table_model_(str_table_name);
		if (nullptr == pmodel)
		{
			return 2;
		}

		mapTableColumn map_column_type = table_column_type_(pmodel, str_table_name);

		/// ---------------------------------------------------------------------------------------
		/// 3. 主键也有效， 直接读取数据
		/// ---------------------------------------------------------------------------------------
		/// 得到当前表列数
		const int column_total_count = map_column_type.count();
		/// 检查参数中的主键索引
		if ((0 > ui_key_column_index) || (ui_key_column_index > column_total_count))
		{
			return 3;
		}

		/// 检查列的最大数是否在表格中
		{
			for (auto item : map_read_except_column_index)
			{
				/// 如果参数中传递的列大于当前表格的最大值，则pass
				if (item.first > (uint)column_total_count)
				{
					return 3;
				}
			}
		}

		/// ---------------------------------------------------------------------------------------
		/// 将数据读入pdata中: 逐行读取， 逐行的同时，读取逐列
		int row_total_count = tableRowCount(pmodel, str_table_name);

		QSqlQuery sql_query = pmodel->query();
		sql_query.first();



		try
		{
			/// 读取行
			for (int row_index = 0; row_index < row_total_count; ++row_index)
			{
				/// ---------------------------------------------------------------------------------------
				/// 这里开始插入map
				TValue map_item_value;
				TKey map_item_key = 0;

				void* pdata = (void*)&map_item_value;

				/// 读取列
				for (int column_index = 0; column_index < column_total_count; ++column_index)
				{

					void* pread_data = nullptr;
					/// 当前列是否为主键列
					bool current_column_is_key = false;

					/// 找到了主键列
					if (column_index == ui_key_column_index)
					{
						pread_data = (void*)&map_item_key;
						current_column_is_key = true;
					}
					/// 不是主键， 读取数据列
					else
					{
						/// 如果当前列是参数map中不需要读取的列， 继续找
						auto find_read_column_index_it = map_read_except_column_index.find((uint)column_index);

						/// 不是当前需要读取的列， 则继续找下一列
						if (find_read_column_index_it != map_read_except_column_index.end())
						{
							continue;
						}

						pread_data = pdata;
					}

					QString str_cur_column_type = map_column_type.value(column_index).str_type_;

					/// 当前类型为空，则跳过当前行
					if (true == str_cur_column_type.isEmpty())
					{
						break;
					}

					/// 如果当前的数据类型不在给定的解析列表中，则pass
					auto find_current_type_is_exist = map_analysis_db_type_.find(str_cur_column_type);
					/// 不在给定的类型中， 则pass
					if (find_current_type_is_exist == map_analysis_db_type_.end())
					{
						break;
					}

					/// 根据对应的数据类型做转换
					if (find_current_type_is_exist->second.type_trans_func_)
					{
						pread_data = (this->*find_current_type_is_exist->second.type_trans_func_)(pread_data, sql_query.value(column_index), !current_column_is_key);
					}
					else
					{
						;/// 数据类型处理函数无效
					}



					/// 如果是主键，则将结果赋值给主键
					if (current_column_is_key)
					{
						map_item_key = *((TKey*)pread_data);
					}
					/// 不是主键， 则将数据保存
					else
					{
						pdata = pread_data;
					}
				}

				///  一行读取结束， 将其插入map
				pout_map->emplace(map_item_key, map_item_value);


				/// 读取下一行, 无法读取，则pass
				if (false == sql_query.next())
				{
					break;
				}
			}
		}
		catch (...)
		{

		}


		return 0;
	}


	/// --------------------------------------------------------------------------------
	/// @brief: oct_sqlite::SqliteExt::update_table_map_some_columns_
	/// --------------------------------------------------------------------------------
	template<typename TKey, typename TValue>
	int oct_sqlite::SqliteExt::update_table_map_some_columns_(const QString str_table_name, const std::map<TKey, TValue>* pmap_data, const uint key_column_index, const MapUIntUInt& map_update_column_index) noexcept
	{

		/// 1. 检查 参数 pmap_data
		if (nullptr == pmap_data)
		{
			return 1;
		}

		/// 2, 获取对应表格的model
		QSqlQueryModel* pmodel = table_model_(str_table_name);
		if (nullptr == pmodel)
		{
			return 2;
		}


		/// 3. 将数据写入数据库, 逐个单元格写入
		/// UPDATE 表名称 SET 列名称 = 新值 WHERE 列名称 = 某值
		/// UPDATE Person SET FirstName = 'Fred' WHERE LastName = 'Wilson' 
		/// UPDATE Person SET Address = 'Zhongshan 23', City = 'Nanjing'
		///							WHERE LastName = 'Wilson'

		/// 5. 获取表的列
		mapTableColumn map_table_column_name = table_column_type_(pmodel, str_table_name);

		/// 6. 检查列
		const uint table_column_total_count = map_table_column_name.count();
		if ((0 == table_column_total_count) || (key_column_index > table_column_total_count))
		{
			return 3;
		}

		/// 检查写入列是否越界
		{
			for (auto item : map_update_column_index)
			{
				/// 如果列的索引大于最大列数， 则返回
				if (table_column_total_count <= item.first)
				{
					return 9;
				}
			}

		}

		/// 7. 获取表的行数， 用于确定要写多少个单元格
		int row_total_count = tableRowCount(pmodel, str_table_name);
		/// 如果表的行为0
		if (0 >= row_total_count)
		{
			return 5;
		}

		/// 8. 如果参数中map的个数与当前数据库中行的个数不等，则返回
		if (row_total_count != pmap_data->size())
		{
			return 6;
		}


		/// 9. 将数据写入数据库
		try
		{
			/// 一共有这么多行的数据需要写入
			for (auto find_value_it = pmap_data->begin(); find_value_it != pmap_data->end(); ++find_value_it)
			{
				void* pkeyData = (void*)&find_value_it->first;

				QString str_update_sql = "UPDATE " + str_table_name + " SET ";
				/// 主键列名
				QString str_key_name = map_table_column_name.value(key_column_index).str_name_;
				QString str_key_type = map_table_column_name.value(key_column_index).str_type_;
				QString str_where_value = QVariant(find_value_it->first).toString();

				/// 是否读取下一行
				bool is_read_next_line = false;

				/// 统计set 列数
				int set_index = 0;

				/// 列数据的起始地址
				void* pdata = (void*)&find_value_it->second;
				/// 每一行有这么多列的数据需要写入
				for (int column_index = 0; column_index < table_column_total_count; ++column_index)
				{
					/// 不能更新主键列
					if (key_column_index == column_index)
					{
						bool is_ok = false;
						column_sql_str_(str_key_type, pkeyData, &str_where_value, is_ok, false);
						if (false == is_ok)
						{
							return 7;
						}

						continue;
					}

					/// 检查是否为写入列
					auto find_update_column_it = map_update_column_index.find((uint)column_index);
					/// 不是需要写入的列，则继续找
					if (find_update_column_it == map_update_column_index.end())
					{
						continue;
					}


					/// 第 X列的名称: id， name, value
					QString str_column_type = map_table_column_name.value(column_index).str_type_;
					QString str_column_name = map_table_column_name.value(column_index).str_name_;

					/// 拼接sql: set column=value
					//pdata = column_sql_str_(str_column_type, pdata, &str_value);
					QString str_value;
					bool is_ok = false;
					pdata = column_sql_str_(str_column_type, pdata, &str_value, is_ok, true);
					if (false == is_ok)
					{
						return 8;
					}

					/// 无法解析
					if (true == str_value.isEmpty())
					{
						is_read_next_line = true;
						break;
					}

					/// 如果这是第二次执行， 则需要添加 逗号
					if (0 < set_index)
					{
						str_update_sql += ", " + str_column_name + "=" + str_value;
						++set_index;
					}
					else
					{
						str_update_sql += str_column_name + "=" + str_value;
						++set_index;
					}
				}	/// end 总列数



					/// 如果不是读取下一行
				if (false == is_read_next_line)
				{
					/// 构建完整的sql语句
					str_update_sql += " WHERE " + str_key_name + "=" + str_where_value + ";";

					//qDebug() << "sql_str=" << str_update_sql;

					///  执行sql语句
					QSqlQuery sql_query = pmodel->query();
					sql_query.first();

					bool prepare_ret = sql_query.prepare(str_update_sql);

					/// 语法错误
					if (false == prepare_ret)
					{
						;
					}
					else
					{
						bool exec_ret = sql_query.exec();
						//qDebug() << "exec_ret=" << exec_ret;
					}
				}/// 读取下一行


				 /// 上面的一行已经写入完成， 下面开始写入下一行数据
			}	/// 有这么多行数据
		}
		catch (...)
		{
			;
		}

		return 0;
	}



	/// --------------------------------------------------------------------------------
	/// @brief: oct_sqlite::SqliteExt::update_table_map_except_columns_
	/// --------------------------------------------------------------------------------
	template<typename TKey, typename TValue>
	int oct_sqlite::SqliteExt::update_table_map_except_columns_(const QString str_table_name, const std::map<TKey, TValue>* pmap_data, const uint key_column_index, const MapUIntUInt& map_except_column_index) noexcept
	{
		/// 1. 检查 参数 pmap_data
		if (nullptr == pmap_data)
		{
			return 1;
		}

		/// 2, 获取对应表格的model
		QSqlQueryModel* pmodel = table_model_(str_table_name);
		if (nullptr == pmodel)
		{
			return 2;
		}


		/// 3. 将数据写入数据库, 逐个单元格写入
		/// UPDATE 表名称 SET 列名称 = 新值 WHERE 列名称 = 某值
		/// UPDATE Person SET FirstName = 'Fred' WHERE LastName = 'Wilson' 
		/// UPDATE Person SET Address = 'Zhongshan 23', City = 'Nanjing'
		///							WHERE LastName = 'Wilson'

		/// 5. 获取表的列
		mapTableColumn map_table_column_name = table_column_type_(pmodel, str_table_name);

		/// 6. 检查列
		const uint table_column_total_count = map_table_column_name.count();
		if ((0 == table_column_total_count) || (key_column_index > table_column_total_count))
		{
			return 3;
		}

		/// 检查写入列是否越界
		{
			for (auto item : map_except_column_index)
			{
				/// 如果列的索引大于最大列数， 则返回
				if (table_column_total_count <= item.first)
				{
					return 6;
				}
			}

		}

		/// 7. 获取表的行数， 用于确定要写多少个单元格
		int row_total_count = tableRowCount(pmodel, str_table_name);
		/// 如果表的行为0
		if (0 >= row_total_count)
		{
			return 5;
		}

		/// 8. 如果参数中map的个数与当前数据库中行的个数不等，则返回
		if (row_total_count != pmap_data->size())
		{
			return 6;
		}


		/// 9. 将数据写入数据库
		try
		{
			/// 一共有这么多行的数据需要写入
			for (auto find_value_it = pmap_data->begin(); find_value_it != pmap_data->end(); ++find_value_it)
			{

				void* pkeyData = (void*)&find_value_it->first;

				QString str_update_sql = "UPDATE " + str_table_name + " SET ";
				/// 主键列名
				QString str_key_name = map_table_column_name.value(key_column_index).str_name_;
				QString str_key_type = map_table_column_name.value(key_column_index).str_type_;
				QString str_where_value;/// = where_value(str_key_name, pkey_value);

										/// 是否读取下一行
				bool is_read_next_line = false;

				/// 统计set 列数
				int set_index = 0;

				/// 列数据的起始地址
				void* pdata = (void*)&find_value_it->second;

				/// 每一行有这么多列的数据需要写入
				for (int column_index = 0; column_index < table_column_total_count; ++column_index)
				{

					QString str_value;
					/// 不能更新主键列
					if (key_column_index == column_index)
					{

						bool is_ok = false;
						column_sql_str_(str_key_type, pkeyData, &str_where_value, is_ok, false);
						if (false == is_ok)
						{
							return 7;
						}

						continue;
					}

					/// 检查是否为不需要写入列
					auto find_update_column_it = map_except_column_index.find((uint)column_index);
					/// 是不需要写入的列，则继续找
					if (find_update_column_it != map_except_column_index.end())
					{
						continue;
					}


					/// 第 X列的名称: id， name, value
					QString str_column_type = map_table_column_name.value(column_index).str_type_;
					QString str_column_name = map_table_column_name.value(column_index).str_name_;

					/// 拼接sql: set column=value

					bool is_ok = false;
					pdata = column_sql_str_(str_column_type, pdata, &str_value, is_ok, true);
					if (false == is_ok)
					{
						return 8;
					}


					/// 无法解析
					if (true == str_value.isEmpty())
					{
						is_read_next_line = true;
						break;
					}

					/// 如果这是第二次执行， 则需要添加 逗号
					if (0 < set_index)
					{
						str_update_sql += ", " + str_column_name + "=" + str_value;
						++set_index;
					}
					else
					{
						str_update_sql += str_column_name + "=" + str_value;
						++set_index;
					}
				}	/// end 总列数



					/// 如果不是读取下一行
				if (false == is_read_next_line)
				{
					/// 构建完整的sql语句
					str_update_sql += " WHERE " + str_key_name + "=" + str_where_value + ";";

					//qDebug() << "sql_str=" << str_update_sql;

					///  执行sql语句
					QSqlQuery sql_query = pmodel->query();
					sql_query.first();

					bool prepare_ret = sql_query.prepare(str_update_sql);

					/// 语法错误
					if (false == prepare_ret)
					{
						;
					}
					else
					{
						bool exec_ret = sql_query.exec();
						//qDebug() << "exec_ret=" << exec_ret;
					}
				}/// 读取下一行


				 /// 上面的一行已经写入完成， 下面开始写入下一行数据
			}	/// 有这么多行数据
		}
		catch (...)
		{
			;
		}

		return 0;
	}


	/// --------------------------------------------------------------------------------
	/// @brief: oct_sqlite::SqliteExt::column_sql_str_T_
	/// --------------------------------------------------------------------------------
	template<typename TDataType, typename TDestType>
	TDataType* oct_sqlite::SqliteExt::TColumn_sql_str_(TDataType* pdata, QString* pout_value, const bool is_move_pdata) noexcept
	{
		TDataType* ret_value = pdata;
		try
		{
			TDestType* pint = (TDestType*)pdata;
			*pout_value = QString::number(*pint);
			if (true == is_move_pdata)
			{
				++pint;
			}

			ret_value = (void*)pint;
		}
		catch (...)
		{
			;
		}

		return ret_value;
	}

}