#include <QFile>
#include <QHeaderView>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include "SqliteExt.h"
#include "SqliteExt.inl"

namespace oct_sqlite
{


	SqliteExt::SqliteExt(QObject* parent) noexcept
		//: QObject(parent)
	{
		str_data_base_connect_name_ = QString("");
		initDataBase();
		init_analysis_db_data_type_();
		initOther();
	}

	SqliteExt::~SqliteExt()
	{
		unInit();
	}

	/// --------------------------------------------------------------------------------
	/// @brief: CommonSql::initialized_
	/// --------------------------------------------------------------------------------
	int SqliteExt::initialize(const stSQLInit& st_init) noexcept
	{
		{
			///1. 参数检查
			int ret = preInit(st_init);
			if (0 != ret)
			{
				return ret;
			}
		}

		/// 2. 避免多次初始化
		unInit();

		/// 3. 尝试打开数据库文件
		sql_data_base_.setDatabaseName(st_init.str_sqlite_file_);

		/// 打开失败
		if (false == sql_data_base_.open())
		{
			QSqlError error = sql_data_base_.lastError();
			return 5;
		}

		{
			/// 5. 打开成功，读取数据库中数据
			/// 读取表名
			QStringList list_table_name = sql_data_base_.tables(static_cast<QSql::TableType>(st_init.en_tt_));

			/// 6. 如果没有表
			if (0 == list_table_name.count())
			{
				return 6;
			}

			/// 7. 存在， 创建对应的sqlmodel和tableview， 
			for (auto str_table_name : list_table_name)
			{
				stTableModelView st_tmv;

				/// 8, 创建tableview
				st_tmv.pview_ = new(std::nothrow) QTableView;
				st_tmv.pmodel_ = new(std::nothrow) QSqlTableModel(st_tmv.pview_, sql_data_base_);

				QSqlTableModel* pmodel = st_tmv.pmodel_;
				/// 9. 设置model的参数
				if (pmodel)
				{
					pmodel->setTable(str_table_name);
					/// 设置编辑策略
					pmodel->setEditStrategy(static_cast<QSqlTableModel::EditStrategy>(st_init.en_es_));
					/// 查询山上
					pmodel->select();
					/// 解除256行的限制
					while (pmodel->canFetchMore())
					{
						pmodel->fetchMore();
					}
				}

				st_tmv.setModelToView();
				/// 10. 设置view的参数
				QTableView* pview = st_tmv.pview_;
				if (pview)
				{
					pview->verticalHeader()->setVisible(false);
					pview->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
					pview->horizontalHeader()->setStretchLastSection(true);
					pview->setSelectionBehavior(QAbstractItemView::SelectItems);
				}

				/// 加入map
				map_db_table_.insert(str_table_name, st_tmv);
			}
		}

		/// 11. 读取结束， 检查map中的个数
		if (0 == map_db_table_.count())
		{
			return 7;
		}

		/// 12, 保存当前操作的数据库名称
		str_data_base_connect_name_ = st_init.str_sqlite_file_;

		return 0;

	}

	/// --------------------------------------------------------------------------------
	/// @brief: 执行sqlite语句
	/// --------------------------------------------------------------------------------
	int SqliteExt::carryOnSql(const QString& str_sql, const QString& str_table_name) noexcept
	{
		/// 1. 如果数据库没有打开，则返回
		if (false == sql_data_base_.isOpen())
		{
			return 1;
		}


		/// 2. 参数为空
		if (true == str_sql.isEmpty())
		{
			return 2;
		}



		/// 3. 执行sql
		auto find_sql_model_it = map_db_table_.find(str_table_name);
		/// 表名不存在
		if (find_sql_model_it == map_db_table_.end())
		{
			return 3;
		}

		QSqlQueryModel* pmodel = find_sql_model_it->pmodel_;
		if (nullptr == pmodel)
		{
			/// 指针无效
			return 5;
		}

		bool prepare_ret = pmodel->query().prepare(str_sql);

		/// 语法错误
		if (false == prepare_ret)
		{
			return 6;
		}

		/// 
		bool exec_ret = pmodel->query().exec();

		/// 返回执行结果
		return (true == exec_ret ? 0 : 7);
	}

	/// --------------------------------------------------------------------------------
	/// @brief: CommonSql::sql_record_
	/// --------------------------------------------------------------------------------
	QSqlQuery* SqliteExt::sqlRecord(const QString& str_sql, const QString& str_table_name) noexcept
	{
		/// 1. 如果数据库没有打开，则返回
		if (false == sql_data_base_.isOpen())
		{
			return nullptr;
		}


		/// 2. 参数为空
		if (true == str_sql.isEmpty())
		{
			return nullptr;
		}



		/// 3. 执行sql
		auto find_sql_model_it = map_db_table_.find(str_table_name);
		/// 表名不存在
		if (find_sql_model_it == map_db_table_.end())
		{
			return nullptr;
		}

		QSqlQueryModel* pmodel = find_sql_model_it->pmodel_;
		if (nullptr == pmodel)
		{
			/// 指针无效
			return nullptr;
		}

		bool prepare_ret = pmodel->query().prepare(str_sql);

		/// 语法错误
		if (false == prepare_ret)
		{
			return nullptr;
		}

		/// 
		bool exec_ret = pmodel->query().exec();

		/// 返回执行结果
		return (true == exec_ret ? &pmodel->query() : nullptr);
	}

	/// --------------------------------------------------------------------------------
	/// @brief: CommonSql::uninitialized_
	/// --------------------------------------------------------------------------------
	void SqliteExt::unInit() noexcept
	{
		try
		{
			for (auto item : map_db_table_)
			{
				item.releaseModelView();
			}

			/// 重置map
			if (0 < map_db_table_.count())
			{
				MapTableModelView map_tmp;
				map_db_table_.swap(map_tmp);
			}

			/// 关闭数据库
			if (true == sql_data_base_.isOpen())
			{
				sql_data_base_.close();
			}

		}
		catch (...)
		{
			;
		}
	}

	/// --------------------------------------------------------------------------------
	/// @brief: CommonSql::map_table_
	/// --------------------------------------------------------------------------------
	const oct_sqlite::MapTableModelView& SqliteExt::dbTableData()  noexcept
	{
		return map_db_table_;
	}


	/// --------------------------------------------------------------------------------
	/// @brief: SqliteAssistant::table_model_
	/// --------------------------------------------------------------------------------
	QSqlTableModel* SqliteExt::table_model_(const QString& str_table_name) noexcept
	{
		/// 1. 找到对应的表
		auto find_model_it = map_db_table_.find(str_table_name);
		if (find_model_it == map_db_table_.end())
		{
			return nullptr;
		}

		/// 3. 获取数据库表的字段类型
		return find_model_it->pmodel_;;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: SqliteAssistant::table_column_name_
	/// --------------------------------------------------------------------------------
	mapTableColumn SqliteExt::table_column_type_(QSqlQueryModel* pModel, const QString& str_table_name)noexcept
	{
		mapTableColumn ret_map;
		if (nullptr == pModel)
		{
			return ret_map;
		}



		QString str_sql_column_type = QString("PRAGMA table_info( " + str_table_name + " ) ");
		QSqlQuery  sqlquery = pModel->query();

		bool ret = sqlquery.prepare(str_sql_column_type);
		if (false == ret)
		{
			return ret_map;
		}


		if (true == sqlquery.exec())
		{
			/// 7. 将列的数据类型放入list
			uint index = 0;
			while (sqlquery.next())
			{
				stTableColumn map_insert_value;
				map_insert_value.str_name_ = sqlquery.value(1).toString();
				map_insert_value.str_type_ = sqlquery.value(2).toString();


				ret_map.insert(index, map_insert_value);
				++index;
			}
		}


		sqlquery.first();

		return ret_map;
	}


	/// --------------------------------------------------------------------------------
	/// @brief: SqliteAssistant::table_row_count_
	/// --------------------------------------------------------------------------------
	int SqliteExt::tableRowCount(QSqlQueryModel* pmodel, const QString& str_table_name)noexcept
	{
		if (nullptr == pmodel)
		{
			return 0;
		}

		/// 继续读取行数
		QSqlQuery sql_query = pmodel->query();
		QString str_sql_query = QString("SELECT * FROM %1").arg(str_table_name);
		sql_query.prepare(str_sql_query);
		QSqlRecord table_record = sql_query.record();
		if (false == sql_query.exec())
		{
			return 7;
		}

		/// 8. 读取行数
		int row_total_count = 0;
		sql_query.first();
		if (true == sql_query.last())
		{
			row_total_count = sql_query.at() + 1;
			sql_query.first();
		}

		return row_total_count;
	}


	/// --------------------------------------------------------------------------------
	/// @brief: 读取str_table_name中的数据到pdata中
	/// --------------------------------------------------------------------------------
	int SqliteExt::table_data_(const QString& str_table_name, void* pdata) noexcept
	{
		/// 0. 指针无效
		if (nullptr == pdata)
		{
			return 1;
		}

		/// 1. 得到数据库模型
		QSqlQueryModel* pmodel = table_model_(str_table_name);
		if (nullptr == pmodel)
		{
			return 2;
		}


		/// ---------------------------------------------------------------------------------------
		/// 5. 准备查询数据库字段的sql语句

		/// 6. 读取表的列数
		mapTableColumn		map_column_type = table_column_type_(pmodel, str_table_name);

		/// 得到当前表列数
		const int column_total_count = map_column_type.count();


		/// ---------------------------------------------------------------------------------------
		/// 将数据读入pdata中: 逐行读取， 逐行的同时，读取逐列
		int row_total_count = tableRowCount(pmodel, str_table_name);
		QSqlQuery sql_query = pmodel->query();
		sql_query.first();

		/// 读取行
		for (int row_index = 0; row_index < row_total_count; ++row_index)
		{
			/// 读取列
			for (int column_index = 0; column_index < column_total_count; ++column_index)
			{
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

			/// 读取下一行, 无法读取，则pass
			if (false == sql_query.next())
			{
				break;
			}
		}

		return 0;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 读取表格str_table_name中map_read_column列的数据到pdata中
	/// --------------------------------------------------------------------------------
	int SqliteExt::table_data_some_columns_(const QString& str_table_name, void* pdata, const MapUIntUInt& map_read_column) noexcept
	{
		/// 0. 指针无效
		if (nullptr == pdata)
		{
			return 1;
		}

		/// 1. 得到数据库模型
		QSqlQueryModel* pmodel = table_model_(str_table_name);
		if (nullptr == pmodel)
		{
			return 2;
		}


		/// ---------------------------------------------------------------------------------------
		/// 5. 准备查询数据库字段的sql语句

		/// 6. 读取表的列数
		mapTableColumn		map_column_type = table_column_type_(pmodel, str_table_name);

		/// 得到当前表列数
		const int column_total_count = map_column_type.count();

		/// 检查列的最大数是否在表格中
		{
			for (MapUIntUInt::iterator columnIt = ((MapUIntUInt)map_read_column).begin(); columnIt != map_read_column.end(); ++columnIt)
			{
				/// 如果参数中传递的列大于当前表格的最大值，则pass
				if (columnIt.key() > (uint)column_total_count)
				{
					return 3;
				}
			}
		}

		/// 参数中的列索引正确， 开始读取

		/// ---------------------------------------------------------------------------------------
		/// 将数据读入pdata中: 逐行读取， 逐行的同时，读取逐列
		int row_total_count = tableRowCount(pmodel, str_table_name);
		QSqlQuery sql_query = pmodel->query();
		sql_query.first();

		/// 读取行
		for (int row_index = 0; row_index < row_total_count; ++row_index)
		{
			/// 读取列
			for (int column_index = 0; column_index < column_total_count; ++column_index)
			{
				/// 如果当前列是参数map指定需要读取的列， 再读取
				auto find_read_column_index_it = map_read_column.find((uint)column_index);

				/// 不是当前需要读取的列， 则继续找下一列
				if (find_read_column_index_it == map_read_column.end())
				{
					continue;
				}

				QString str_cur_column_type = map_column_type.value(column_index).str_type_;

				/// 如果当前的数据类型不在给定的解析列表中，则pass
				auto find_current_type_is_exist = map_analysis_db_type_.find(str_cur_column_type);
				/// 不在给定的类型中， 则pass
				if (find_current_type_is_exist == map_analysis_db_type_.end())
				{
					return 5;
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

			/// 读取下一行, 无法读取，则pass
			if (false == sql_query.next())
			{
				break;
			}
		}

		return 0;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: CommonSql::pre_initialized_
	/// --------------------------------------------------------------------------------
	int SqliteExt::preInit(const stSQLInit& st_init) noexcept
	{
		/// 1. 数据库文件不存在
		{
			QFile file_db(st_init.str_sqlite_file_);
			if (false == file_db.exists())
			{
				return 1;
			}

			/// 2. 检查参数
			const uint en_tt = st_init.en_tt_;
			if ((TT_TABLES != en_tt) && (TT_VIEWS != en_tt) && (TT_SYSTEM_TABLES != en_tt) && (TT_ALL_TABLES != en_tt))
			{
				return 2;
			}

			/// 检查第三个参数
			if ((ES_FILE_CHANGE != st_init.en_es_) && (ES_ROW_CHANGE != st_init.en_es_) && (ES_MANUAL_SUBMIT != st_init.en_es_))
			{
				return 3;
			}
		}

		return 0;

	}

	/// --------------------------------------------------------------------------------
	/// @brief: CommonSql::initialize_data_base_
	/// --------------------------------------------------------------------------------
	void SqliteExt::initDataBase() noexcept
	{
		bool is_contain = QSqlDatabase::contains(str_data_base_connect_name_);
		if (true == is_contain)
		{
			sql_data_base_ = QSqlDatabase::database(str_data_base_connect_name_);
		}
		else
		{
			sql_data_base_ = QSqlDatabase::addDatabase("QSQLITE", str_data_base_connect_name_);
			str_data_base_connect_name_ = "QSQLITE";
		}
	}



	/// --------------------------------------------------------------------------------
	/// @brief: 解析数据库默认数据类型
	/// --------------------------------------------------------------------------------
	void SqliteExt::init_analysis_db_data_type_() noexcept
	{

		auto insert_map = [&](const QString str_type, type_transfer_func func, column_sql_str_func update_sql_func, enDBColumnType en_dbct)
		{
			stTypeTransferFunc map_item_value;
			map_item_value.type_trans_func_ = func;
			map_item_value.enType_ = en_dbct;
			map_item_value.update_type_sql_func_ = update_sql_func;

			map_analysis_db_type_.emplace(str_type, map_item_value);
		};

		/// 1. 文本
		insert_map("TEXT", &SqliteExt::analysis_cell_value_str_, &SqliteExt::column_sql_str_text_, DBCT_TEXT);
		/// 2. 整数
		insert_map("INTEGER", &SqliteExt::analysis_cell_value_int_, &SqliteExt::column_sql_str_integer_, DBCT_INT);
		/// 3. double
		insert_map("DOUBLE", &SqliteExt::analysis_cell_value_double_, &SqliteExt::column_sql_str_double_, DBCT_DOUBLE);
		/// 5. char 
		insert_map("BOOLEAN", &SqliteExt::analysis_cell_value_bool_, &SqliteExt::column_sql_str_boolean_, DBCT_BOOLEAN);
		/// 6. uint
		insert_map("UINT", &SqliteExt::analysis_cell_value_uint_, &SqliteExt::column_sql_str_uint_, DBCT_UINT);
		/// 7. ushort
		insert_map("USHORT", &SqliteExt::analysis_cell_value_ushort_, &SqliteExt::column_sql_str_ushort_, DBCT_USHORT);
		/// 8. short
		insert_map("SHORT", &SqliteExt::analysis_cell_value_short_, &SqliteExt::column_sql_str_short_, DBCT_SHORT);




	}

	/// --------------------------------------------------------------------------------
	/// @brief: CommonSql::analysis_cell_value_int_
	/// --------------------------------------------------------------------------------
	void* SqliteExt::analysis_cell_value_int_(void* pdata, const QVariant& var_data, const bool is_move_data_ptr) noexcept
	{
		try
		{
			if (nullptr == pdata)
			{
				return pdata;
			}

			int* pvalue = (int*)(pdata);
			*pvalue = var_data.toInt();

			if (true == is_move_data_ptr)
			{
				++pvalue;
			}

			pdata = (void*)pvalue;
		}
		catch (...)
		{

		}

		return pdata;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: CommonSql::analysis_cell_value_str_
	/// --------------------------------------------------------------------------------
	void* SqliteExt::analysis_cell_value_str_(void* pdata, const QVariant& var_data, const bool is_move_data_ptr) noexcept
	{
		if (nullptr == pdata)
		{
			return pdata;
		}
		QString* pvalue = (QString*)(pdata);

		try
		{
			*pvalue = var_data.toString();
			if (is_move_data_ptr)
			{
				++pvalue;
			}

			pdata = (void*)pvalue;

		}
		catch (...)
		{

		}

		return pdata;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: CommonSql::analysis_cell_value_double_
	/// --------------------------------------------------------------------------------
	void* SqliteExt::analysis_cell_value_double_(void* pdata, const QVariant& var_data, const bool is_move_data_ptr) noexcept
	{

		if (nullptr == pdata)
		{
			return pdata;
		}

		double* pvalue = (double*)(pdata);

		try
		{
			*pvalue = var_data.toDouble();
			if (is_move_data_ptr)
			{
				++pvalue;
			}

			pdata = (void*)pvalue;
		}
		catch (...)
		{

		}

		return pdata;
	}


	/// --------------------------------------------------------------------------------
	/// @brief: CommonSql::读取当前数据库中名为str_table_name的参数
	/// --------------------------------------------------------------------------------
	int SqliteExt::table(const QString str_table_name, stTableModelView* pout_tmv) noexcept
	{
		/// 1. 参数为空无法存放读取结果
		if (nullptr == pout_tmv)
		{
			return 1;
		}


		/// 2. 找参数中的表格
		auto find_table_it = map_db_table_.find(str_table_name);

		/// 3. 没找到； 返回2
		if (find_table_it == map_db_table_.end())
		{
			return 2;
		}

		/// 5 找到了，返回结果
		*pout_tmv = find_table_it.value();

		return 0;
	}


#ifdef BUG_CODE
	/// --------------------------------------------------------------------------------
	/// @brief: 将数据写入数据库
	/// --------------------------------------------------------------------------------
	int SqliteExt::update_table_(const QString str_table_name, void* pdata, const uint key_column_index) noexcept
	{
		/// 1. 检查 参数 pdata
		if (nullptr == pdata)
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

		/// 7. 获取表的行数， 用于确定要写多少个单元格
		int row_total_count = tableRowCount(pmodel, str_table_name);
		/// 如果表的行为0
		if (0 >= row_total_count)
		{
			return 5;
		}

		try
		{
			/// ---------------------------------------------------------------------------------------
			/// 有这么多行
			for (int row_index = 0; row_index < row_total_count; ++row_index)
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



				/// 有这么多列
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
							return 6;
						}

						continue;
					}

					/// 第 X列的名称: id， name, value
					QString str_column_type = map_table_column_name.value(column_index).str_type_;
					QString str_column_name = map_table_column_name.value(column_index).str_name_;


					bool is_ok = false;
					pdata = column_sql_str_(str_column_type, pdata, &str_value, is_ok, true);
					if (false == is_ok)
					{
						return 7;
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
				}

				/// 如果不是读取下一行
				if (false == is_read_next_line)
				{
					/// 构建完整的sql语句
					str_update_sql += " WHERE " + str_key_name + "=" + str_where_value + ";";

					///  执行sql语句
					QSqlQuery sql_query = pmodel->query();
					sql_query.first();

					bool prepare_ret = sql_query.prepare(str_update_sql);

					/// 语法错误
					if (false == prepare_ret)
					{
						qDebug() << "error_sql=" << str_update_sql;
					}
					else
					{
						bool exec_ret = sql_query.exec();
						qDebug() << "exec_ret=" << exec_ret;
					}
				}
				/// 继续下一行
			}

		}
		catch (...)
		{

		}



		return 0;
	}

#endif /// BUG_CODE


	/// --------------------------------------------------------------------------------
	/// @brief: SqliteExt::init_other_
	/// --------------------------------------------------------------------------------
	void SqliteExt::initOther()noexcept
	{
		/// 参数1: sqlite的后缀, 比如: .slite
		auto insert_map = [=](const QString& str_suffix)
		{
			hash_sql_file_suffix_.insert(str_suffix, 0);
		};

		insert_map(".db");
		insert_map(".sdb");
		insert_map(".sqlite");
		insert_map(".db3");
		insert_map(".s3db");
		insert_map(".sqlite3");
		insert_map(".sl3");
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 检查文件后缀是否为可支持类型
	/// --------------------------------------------------------------------------------
	bool SqliteExt::file_suffix_is_right_(const QString& str_suffix) noexcept
	{
		auto find_suffix_it = hash_sql_file_suffix_.find(str_suffix);
		if (find_suffix_it == hash_sql_file_suffix_.end())
		{
			return false;
		}

		return true;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: SqliteExt::submit_
	/// --------------------------------------------------------------------------------
	int SqliteExt::submitTableData(const QString& str_table_name) noexcept
	{
		auto find_table_it = map_db_table_.find(str_table_name);

		/// 1. 没有找到
		if (find_table_it == map_db_table_.end())
		{
			return 1;
		}

		/// 2. 找到了，检查提交结果
		auto pmodel = find_table_it.value().pmodel_;
		if (pmodel)
		{
			if (false == pmodel->submitAll())
			{
				return 3;
			}
		}
		else
		{
			return 2;
		}

		return 0;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 提交所有修改
	/// --------------------------------------------------------------------------------
	int SqliteExt::submitAllTableData() noexcept
	{
		for (auto item : map_db_table_)
		{
			item.submitAll();
		}

		return 0;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 将支持的文件以字符串的形式返回
	/// --------------------------------------------------------------------------------
	QString SqliteExt::dbSuffix() noexcept
	{
		QString str_ret;

		for (auto find_it = hash_sql_file_suffix_.begin(); find_it != hash_sql_file_suffix_.end(); ++find_it)
		{
			str_ret += find_it.key() + " ";
		}

		return str_ret;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: SqliteExt::table_data_except_columns_
	/// --------------------------------------------------------------------------------
	int SqliteExt::table_data_except_columns_(const QString& str_table_name, void* pdata, const MapUIntUInt& map_except_column)noexcept
	{
		/// 0. 指针无效
		if (nullptr == pdata)
		{
			return 1;
		}

		/// 1. 得到数据库模型
		QSqlQueryModel* pmodel = table_model_(str_table_name);
		if (nullptr == pmodel)
		{
			return 2;
		}


		/// ---------------------------------------------------------------------------------------
		/// 5. 准备查询数据库字段的sql语句

		/// 6. 读取表的列数
		mapTableColumn		map_column_type = table_column_type_(pmodel, str_table_name);

		/// 得到当前表列数
		const int column_total_count = map_column_type.count();

		/// 检查列的最大数是否在表格中
		{
			for (auto item : map_except_column)
				for (MapUIntUInt::iterator columnIt = ((MapUIntUInt)map_except_column).begin(); columnIt != map_except_column.end(); ++columnIt)
				{
					/// 如果参数中传递的列大于当前表格的最大值，则pass
					if (columnIt.key() > (uint)column_total_count)
					{
						return 3;
					}
				}
		}

		/// 参数中的列索引正确， 开始读取

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
				/// 读取列
				for (int column_index = 0; column_index < column_total_count; ++column_index)
				{
					/// 找当前行是否为 参数中 禁止读取的列
					auto find_read_column_index_it = map_except_column.find((uint)column_index);

					/// 当前列 是 禁止读取的列， 则找下一列
					if (find_read_column_index_it != map_except_column.end())
					{
						continue;
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

				/// 读取下一行, 无法读取，则pass
				if (false == sql_query.next())
				{
					break;
				}
			}
		}
		catch (...)
		{
			;
		}

		return 0;
	}


#ifdef BUG_CODE

	/// --------------------------------------------------------------------------------
	/// @brief: SqliteExt::update_table_some_columns_
	/// --------------------------------------------------------------------------------
	int SqliteExt::update_table_some_columns_(const QString str_table_name, void* pdata, const uint key_column_index, const MapUIntUInt map_update_column_index)noexcept
	{
		/// 1. 检查 参数 pdata
		if (nullptr == pdata)
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


		/// 检查列是否超过当前最大列
		{
			for (auto item : map_update_column_index)
			{
				/// 如果存在大于最大行的数据，则返回
				if (table_column_total_count < item.first)
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


		try
		{
			/// ---------------------------------------------------------------------------------------
			/// 有这么多行
			for (int row_index = 0; row_index < row_total_count; ++row_index)
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

				/// 有这么多列
				for (int column_index = 0; column_index < table_column_total_count; ++column_index)
				{
					QString str_value;
					/// 不能更新主键列
					if (key_column_index == column_index)
					{
						bool is_ok = false;
						pdata = column_sql_str_(str_key_type, pdata, &str_where_value, is_ok, true);
						if (false == is_ok)
						{
							return 5;
						}

						continue;
					}


					/// 检查当前列是否为写入列
					auto find_write_column_it = map_update_column_index.find((uint)column_index);

					/// 如果不是当前需要写入的列，则继续找
					if (find_write_column_it == map_update_column_index.end())
					{
						continue;
					}



					/// 第 X列的名称: id， name, value
					QString str_column_type = map_table_column_name.value(column_index).str_type_;
					QString str_column_name = map_table_column_name.value(column_index).str_name_;

					/// 拼接sql: set column=value
					bool is_ok = false;
					pdata = column_sql_str_(str_column_type, pdata, &str_where_value, is_ok, true);
					if (false == is_ok)
					{
						return 6;
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
				}

				/// 如果不是读取下一行
				if (false == is_read_next_line)
				{
					/// 构建完整的sql语句
					str_update_sql += " WHERE " + str_key_name + "=" + str_where_value + ";";

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
				}
				/// 继续下一行
			}
		}
		catch (...)
		{

		}


		return 0;
	}


#endif /// BUG_CODE

	/// --------------------------------------------------------------------------------
	/// @brief: SqliteExt::update_table_except_columns_
	/// --------------------------------------------------------------------------------
	int SqliteExt::update_table_except_columns_(const QString str_table_name, void* pdata, const uint key_column_index, const MapUIntUInt map_except_column_index)noexcept
	{
		/// 1. 检查 参数 pdata
		if (nullptr == pdata)
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


		/// 检查列是否超过当前最大列
		{
			for (MapUIntUInt::iterator it = ((MapUIntUInt)map_except_column_index).begin(); it != map_except_column_index.end(); ++it)
			{
				/// 如果存在大于最大行的数据，则返回
				if (table_column_total_count < it.key())
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


		try
		{

			/// ---------------------------------------------------------------------------------------
			/// 有这么多行
			for (int row_index = 0; row_index < row_total_count; ++row_index)
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

				/// 有这么多列
				for (int column_index = 0; column_index < table_column_total_count; ++column_index)
				{
					QString str_value;
					/// 不能更新主键列
					if (key_column_index == column_index)
					{
						bool is_ok = false;
						pdata = column_sql_str_(str_key_type, pdata, &str_where_value, is_ok, true);
						if (false == is_ok)
						{
							return 5;
						}
						continue;
					}


					/// 检查当前列是否为写入列
					auto find_write_column_it = map_except_column_index.find((uint)column_index);

					/// 如果是不需要写入的列，则继续找
					if (find_write_column_it != map_except_column_index.end())
					{
						continue;
					}



					/// 第 X列的名称: id， name, value
					QString str_column_type = map_table_column_name.value(column_index).str_type_;
					QString str_column_name = map_table_column_name.value(column_index).str_name_;

					/// 拼接sql: set column=value
					bool is_ok = false;
					pdata = column_sql_str_(str_column_type, pdata, &str_where_value, is_ok, true);
					if (false == is_ok)
					{
						return 6;
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
				}

				/// 如果不是读取下一行
				if (false == is_read_next_line)
				{
					/// 构建完整的sql语句
					str_update_sql += " WHERE " + str_key_name + "=" + str_where_value + ";";

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
				}
				/// 继续下一行
			}
		}
		catch (...)
		{

		}


		return 0;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: bool
	/// --------------------------------------------------------------------------------
	void* SqliteExt::analysis_cell_value_bool_(void* pdata, const QVariant& var_data, const bool is_move_data_ptr) noexcept
	{
		if (nullptr == pdata)
		{
			return pdata;
		}

		bool* pvalue = (bool*)(pdata);

		try
		{
			*pvalue = var_data.toBool();
			if (is_move_data_ptr)
			{
				++pvalue;
			}
			//++pvalue;

			pdata = (void*)pvalue;
		}
		catch (...)
		{

		}

		return pdata;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: SqliteExt::analysis_cell_value_ushort_
	/// --------------------------------------------------------------------------------
	void* SqliteExt::analysis_cell_value_ushort_(void* pdata, const QVariant& var_data, const bool is_move_data_ptr) noexcept
	{
		if (nullptr == pdata)
		{
			return pdata;
		}

		ushort* pvalue = (ushort*)(pdata);

		try
		{
			*pvalue = var_data.toString().toUShort();
			if (is_move_data_ptr)
			{
				++pvalue;
			}
			//++pvalue;

			pdata = (void*)pvalue;
		}
		catch (...)
		{

		}

		return pdata;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: SqliteExt::analysis_cell_value_short_
	/// --------------------------------------------------------------------------------
	void* SqliteExt::analysis_cell_value_short_(void* pdata, const QVariant& var_data, const bool is_move_data_ptr) noexcept
	{
		if (nullptr == pdata)
		{
			return pdata;
		}

		short* pvalue = (short*)(pdata);

		try
		{
			*pvalue = var_data.toString().toShort();
			if (is_move_data_ptr)
			{
				++pvalue;
			}
			//++pvalue;

			pdata = (void*)pvalue;
		}
		catch (...)
		{

		}

		return pdata;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: SqliteExt::analysis_cell_value_uint_
	/// --------------------------------------------------------------------------------
	void* SqliteExt::analysis_cell_value_uint_(void* pdata, const QVariant& var_data, const bool is_move_data_ptr) noexcept
	{
		if (nullptr == pdata)
		{
			return pdata;
		}

		uint* pvalue = (uint*)(pdata);

		try
		{
			*pvalue = var_data.toUInt();
			if (is_move_data_ptr)
			{
				++pvalue;
			}
			//++pvalue;

			pdata = (void*)pvalue;
		}
		catch (...)
		{

		}

		return pdata;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: SqliteExt::column_sql_str_text_
	/// --------------------------------------------------------------------------------
	void* SqliteExt::column_sql_str_text_(void* pdata, QString* pout_value, const bool is_move_pdata) noexcept
	{
		void* ret_value = pdata;
		try
		{
			QString strrrr = *(QString*)pdata;

			QString* pstr = (QString*)pdata;

			*pout_value = "'" + *pstr + "'";
			if (true == is_move_pdata)
			{
				++pstr;

			}
			//++pstr;
			ret_value = (void*)pstr;
		}
		catch (...)
		{
			;
		}

		return ret_value;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: SqliteExt::column_sql_str_integer_
	/// --------------------------------------------------------------------------------
	void* SqliteExt::column_sql_str_integer_(void* pdata, QString* pout_value, const bool is_move_pdata) noexcept
	{
		return TColumn_sql_str_<void, int>(pdata, pout_value, is_move_pdata);
	}

	/// --------------------------------------------------------------------------------
	/// @brief: SqliteExt::column_sql_str_double_
	/// --------------------------------------------------------------------------------
	void* SqliteExt::column_sql_str_double_(void* pdata, QString* pout_value, const bool is_move_pdata) noexcept
	{
		//return TColumn_sql_str_<void, double> (pdata, pout_value);
		void*& ret_value = pdata;
		try
		{
			double* pdb = (double*)pdata;
			*pout_value = QString::number(*pdb, 'f', 10);
			if (true == is_move_pdata)
			{
				++pdb;
			}
			//++pdb;
			ret_value = (void*)pdb;
		}
		catch (...)
		{
			;
		}

		return ret_value;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: SqliteExt::column_sql_str_boolean_
	/// --------------------------------------------------------------------------------
	void* SqliteExt::column_sql_str_boolean_(void* pdata, QString* pout_value, const bool is_move_pdata) noexcept
	{
		return TColumn_sql_str_<void, bool>(pdata, pout_value, is_move_pdata);
	}

	/// --------------------------------------------------------------------------------
	/// @brief: SqliteExt::column_sql_str_uint_
	/// --------------------------------------------------------------------------------
	void* SqliteExt::column_sql_str_uint_(void* pdata, QString* pout_value, const bool is_move_pdata) noexcept
	{
		return TColumn_sql_str_<void, uint>(pdata, pout_value, is_move_pdata);
	}

	/// --------------------------------------------------------------------------------
	/// @brief: SqliteExt::column_sql_str_ushort_
	/// --------------------------------------------------------------------------------
	void* SqliteExt::column_sql_str_ushort_(void* pdata, QString* pout_value, const bool is_move_pdata) noexcept
	{
		return TColumn_sql_str_<void, ushort>(pdata, pout_value, is_move_pdata);
	}

	/// --------------------------------------------------------------------------------
	/// @brief: SqliteExt::column_sql_str_short_
	/// --------------------------------------------------------------------------------
	void* SqliteExt::column_sql_str_short_(void* pdata, QString* pout_value, const bool is_move_pdata) noexcept
	{
		return TColumn_sql_str_<void, short>(pdata, pout_value, is_move_pdata);
	}

	/// --------------------------------------------------------------------------------
	/// @brief: SqliteExt::column_sql_str_
	/// --------------------------------------------------------------------------------
	void* SqliteExt::column_sql_str_(const QString& str_key_type, void* pdata, QString* pout_value, bool& is_ok, const bool is_move_pdata) noexcept
	{
		auto find_func_it = map_analysis_db_type_.find(str_key_type);
		if (find_func_it == map_analysis_db_type_.end())
		{
			is_ok = false;
			return pdata;
		}

		/// 对应的列转换函数
		column_sql_str_func column_func = find_func_it->second.update_type_sql_func_;
		if (nullptr == column_func)
		{
			is_ok = false;
			return pdata;
		}


		pdata = (this->*column_func)(pdata, pout_value, is_move_pdata);
		is_ok = true;

		return pdata;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 删除参数中表格中的数据
	/// --------------------------------------------------------------------------------
	int SqliteExt::removeTableAllRows(QSqlQueryModel* pmodel, const QString& str_table_name) noexcept
	{
		if (nullptr == pmodel || 0 == str_table_name.trimmed().length())
		{
			return 1;
		}

		/// 继续读取行数
		QSqlQuery sql_query = pmodel->query();
		QString str_sql_query = QString("DELETE FROM %1").arg(str_table_name);

		sql_query.prepare(str_sql_query);
		QSqlRecord table_record = sql_query.record();

		if (false == sql_query.exec())
		{
			return 7;
		}

		return 0;
	}

	/// --------------------------------------------------------------------------------
	//支持的数据库后缀
	/// --------------------------------------------------------------------------------
	QStringList SqliteExt::dbSuffixList() noexcept
	{
		QStringList list_ret;
		{
			for (auto find_it = hash_sql_file_suffix_.begin(); find_it != hash_sql_file_suffix_.end(); ++find_it)
			{
				list_ret << find_it.key();
			}
		}

		return list_ret;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 创建数据库文件并创建表格
	/// --------------------------------------------------------------------------------
	int SqliteExt::newDBFile(const QString& db_file, const QStringList& list_create_sql)
	{
		/// 1. 参数检查
		if (0 == db_file.trimmed().length())
		{
			return 1;
		}

		/// 2. 创建库
		QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
		db.setDatabaseName(db_file);

		/// 3. 打开失败，
		if (false == db.open())
		{
			return 2;
		}

		int ret_value = 0;


		if (0 != list_create_sql.count())
		{
			// 5. 建库
			QSqlQuery sq(db);

			for (const QString& table_create_sql : list_create_sql)
			{
				if (0 == table_create_sql.trimmed().length())
				{
					/// 失败
					ret_value = 3;
					continue;
				}

				const bool&& pre_ret = sq.prepare(table_create_sql);
				if (false == pre_ret)
				{
					const QString error = sq.lastError().text();
					/// 失败
					ret_value = 3;
				}
				/// 成功，执行sql
				else
				{
					const bool&& exec_ret = sq.exec();
					/// 执行失败
					if (false == exec_ret)
					{
						ret_value = 5;
					}
					else
					{
						;
					}
				}

			}
		}
		/// sql语句为空
		else
		{
			ret_value = 6;
		}

		db.close();

		return 0;
	}

}