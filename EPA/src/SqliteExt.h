#ifndef SQLITE_EX_H_
#define SQLITE_EX_H_

#include <QObject>
#include <QMap>
#include <QSqlDatabase>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QMap>
#include <QHash>

class QSqlTableModel;
class QTableView;
namespace oct_sqlite
{

	using HashSqliteSuffix = QHash<QString, uchar>;

	/// ----------------------------------------------------------------------------------------
	/// @brief: 读取类型
	/// ----------------------------------------------------------------------------------------
	enum enTableType
	{
		/// 数据库中的表
		TT_TABLES = 1,
		/// 数据库中的系统表
		TT_SYSTEM_TABLES = 2,
		/// 数据库视图
		TT_VIEWS = 4,
		/// 数据库中的全部表格
		TT_ALL_TABLES = 0xFF,
	};




	enum enEditStrategy
	{
		ES_FILE_CHANGE = 0,
		ES_ROW_CHANGE = 1,
		ES_MANUAL_SUBMIT = 2,
	};


	/// ----------------------------------------------------------------------------------------
	/// @brief: 数据库中的列类型
	/// ----------------------------------------------------------------------------------------
	enum enDBColumnType
	{
		///未知
		DBCT_NONE = 0,

		/// 整形
		DBCT_INT = 1,
		/// TEXT
		DBCT_TEXT = 2,
		/// DOUBLE
		DBCT_DOUBLE = 3,

		/// bool
		DBCT_BOOLEAN = 5,
		/// ushort= unsigned short
		DBCT_USHORT = 6,
		/// SHORT
		DBCT_SHORT = 7,
		/// UINT 
		DBCT_UINT = 8,


	};


	/// ----------------------------------------------------------------------------------------
	/// @brief: 初始化参数
	/// ----------------------------------------------------------------------------------------
	struct stSQLInit_
	{
		/// 数据库文件
		QString str_sqlite_file_;
		/// 读取文件类型
		enTableType en_tt_;
		/// 编辑策略
		enEditStrategy en_es_;
	};
	using stSQLInit = stSQLInit_;

	/// ----------------------------------------------------------------------------------------
	/// @brief: 操作model
	/// ----------------------------------------------------------------------------------------
	struct stTableModelView_
	{
		/// model
		QSqlTableModel* pmodel_ = nullptr;
		/// view
		QTableView* pview_ = nullptr;

		void setModelToView()
		{
			if (pview_)
			{
				pview_->setModel(pmodel_);
			}
		}

		void submitAll()
		{
			if (pmodel_)
			{
				pmodel_->submitAll();
			}
		}

		void releaseModelView()
		{
			if (pmodel_)
			{
				delete pmodel_;
				pmodel_ = nullptr;
			}

			if (pview_)
			{
				delete pview_;
				pview_ = nullptr;
			}

		}
	};

	using stTableModelView = stTableModelView_;

	/// ----------------------------------------------------------------------------------------
	/// @brief: 保存tableName
	/// ----------------------------------------------------------------------------------------
	/// <表名，model_view>
	using MapTableModelView = QMap<QString, stTableModelView>;











	class SqliteExt;
	/// 对应的数据转换函数
	using type_transfer_func = void* (SqliteExt::*)(void* pdata, const QVariant& var_data, const bool is_move_data_ptr);
	/// 对应的类型updatesql语句
	using column_sql_str_func = void* (SqliteExt::*)(void* pdata, QString* pout_value, const bool is_move_data_ptr);

	/// ----------------------------------------------------------------------------------------
	/// @brief: 对应的数据类型处理函数结构体
	/// ----------------------------------------------------------------------------------------
	struct stTypeTransferFunc
	{
		/// 对应的类型处理函数
		type_transfer_func		type_trans_func_ = nullptr;
		/// sql语句更新的处理函数
		column_sql_str_func		update_type_sql_func_ = nullptr;

		/// 当前是哪个数据类型
		enDBColumnType			enType_ = DBCT_NONE;
	};

	/// <列号，对应的数据类型>
	struct stTableColumn
	{
		/// 列名
		QString str_name_ = "";
		/// 列数据类型
		QString str_type_ = "";
	};

	using mapTableColumn = QMap<uint, stTableColumn>;


	/// ----------------------------------------------------------------------------------------
	/// @brief: 
	/// ----------------------------------------------------------------------------------------
	using MapUIntUInt = QMap<uint, uint>;


	//#define BUG_CODE




	/// ----------------------------------------------------------------------------------------
	/// @brief: 通用读取sqlite
	/// ----------------------------------------------------------------------------------------
	class SqliteExt ///: public QObject
	{


	public:
		explicit SqliteExt(QObject* parent = nullptr) noexcept;
		virtual ~SqliteExt();

		/// ----------------------------------------------------------------------------------------
		/// @brief: 屏蔽相关构造函数
		/// ----------------------------------------------------------------------------------------
		SqliteExt(const SqliteExt& instance) = delete;
		SqliteExt(const SqliteExt&& instance) = delete;
		SqliteExt& operator = (const SqliteExt& instance) = delete;
		SqliteExt& operator = (const SqliteExt&& instance) = delete;

		/// -------------------------------------------------------------------------------
		/// @brief:		创建数据库文件并创建表格
		/// @param: 	const QString & db_file - 数据库文件全路径
		/// @param: 	const QString & create_sql - 对应表格创建sql语句
		///  @ret:		int
		///				0 - 建库+执行语句均成功
		///				1 - 失败，db_file 参数为空
		///				2 - 失败， 无法建库
		///				3 - 建库成功， sql语句语法错误
		///				5 - 建库成功， sql语句执行错误
		/// -------------------------------------------------------------------------------
		int newDBFile(const QString& db_file, const QStringList& list_create_sql);

		/// -------------------------------------------------------------------------------
		/// @brief:		初始化
		/// @param: 	const stSQLInit & st_init - 
		///				0 - 成功
		///				1 - 失败， st_init的第一个参数错误， 文件不存在
		///				2 - 失败， st_init的第二个参数错误， 不在给定的值中
		///				3 - 失败， st_init的第三个参数错误， 不在给定的值中
		///				5 - 失败， 无法打开参数中的数据库文件, 检查是否为sqlite3类型的数据库格式
		///				6 - 失败， 数据库中没有表格，空的数据库
		///				7 - 失败， 数据库解析失败，没有表格
		/// -------------------------------------------------------------------------------
		int initialize(const stSQLInit& st_init) noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		执行sqlite语句
		/// @param: 	const QString & str_sql - sql语句
		/// @param: 	const QString & str_table_name - 对数据库的那张表执行 
		///  @ret:		int
		///				0 - 成功
		///				1 - 失败， 数据库没有初始化
		///				2 - 失败， 参数str_sql 为空
		///				3 - 失败， 数据库中没有[str_table_name]这张表
		///				5 - 失败， 对应的数据库model没有得到
		///				6 - 失败， sql语句错误
		///				7 - 失败， sql语句执行失败，请检查sql语句
		/// -------------------------------------------------------------------------------
		int carryOnSql(const QString& str_sql, const QString& str_table_name) noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		获取sql语句执行结果
		/// @param: 	const QString & str_sql - 对应的sql语句
		/// @param: 	const QString & str_table_name - 操作哪张表
		///  @ret:		QT_NAMESPACE::QSqlQuery*
		///					nullptr - sql语句或者没有[str_table_name]对应的表格, 或者创建对应的model错误
		///					!= nullptr - 成功
		/// -------------------------------------------------------------------------------
		QSqlQuery* sqlRecord(const QString& str_sql, const QString& str_table_name) noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		释放
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void unInit() noexcept;


		/// -------------------------------------------------------------------------------
		/// @brief:		获取当数据库中的表
		///  @ret:		oct_sql::mapTable&
		///				
		/// -------------------------------------------------------------------------------
		const MapTableModelView& dbTableData() noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		读取当前数据库中名为str_table_name的参数， 
		/// @param: 	const QString str_table_name - 读取哪张表
		/// @param: 	stTableModelView * pout_tmv - 读取结果存放地址， 内部不会赋值， 外部赋值
		///  @ret:		int
		///				0 - 成功
		///				1 - 失败，参数[pout_tmv]为nullptr
		///				2 - 失败， 数据库中没有名为【str_table_name】的表格
		/// -------------------------------------------------------------------------------
		int table(const QString str_table_name, stTableModelView* pout_tmv) noexcept;


		/// -------------------------------------------------------------------------------
		/// @brief:		读取str_table_name中的数据到pdata
		/// @param: 	const QString & str_table_name - 哪张表
		/// @param: 	void * pdata - 数据存放地址
		///  @ret:		int
		///				0 - 成功
		///				1 - 失败， 参数中的[pdata]为空， 
		///				2 - 失败， 参数中【str_table_name】的表没有在当前的数据库中
		///				3 - 失败， 无法操作当前表的model
		///				5 - 失败， 查询数据库失败
		///				6 - 失败， 参数[str_table_name]的表格为空表
		///				7 - 失败， 无法获取表格的行数
		/// -------------------------------------------------------------------------------
		int table_data_(const QString& str_table_name, void* pdata) noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		读取表格str_table_name中map_read_column列的数据到pdata中
		/// @param: 	const QString & str_table_name - 
		/// @param: 	void * pdata - 
		/// @param: 	const mapUIntUInt & map_read_column - 
		///  @ret:		int
		///				
		/// -------------------------------------------------------------------------------
		int table_data_some_columns_(const QString& str_table_name, void* pdata, const MapUIntUInt& map_read_column) noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		读取表格str_table_name中 不在map_read_column列的数据到pdata中
		/// @param: 	const QString & str_table_name - 
		/// @param: 	void * pdata - 
		/// @param: 	const mapUIntUInt & map_read_column - 
		///  @ret:		int
		///				
		/// -------------------------------------------------------------------------------
		int table_data_except_columns_(const QString& str_table_name, void* pdata, const MapUIntUInt& map_except_column) noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		初始化参数指定为： ES_MANUAL_SUBMIT ， 调用该函数
		/// @param: 	const QString & str_table_name - 哪张表需要提交
		///  @ret:		int
		///				0 - 成功
		///				1 - 失败， 没有找到
		///				2 - 失败，没有对应的model
		///				3 - 失败， 提交失败
		/// -------------------------------------------------------------------------------
		int submitTableData(const QString& str_table_name) noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		提交所有修改
		///  @ret:		int
		///				
		/// -------------------------------------------------------------------------------
		int submitAllTableData() noexcept;


		/// -------------------------------------------------------------------------------
		/// @brief:		读取【str_table_name】中的数据，将结果存放到[pout_map]中
		/// @param: 	const QString str_table_name - 哪张表
		/// @param: 	std::map<TKey,TValue> * pout_map - 存放表数据的map, 外部分配空间， 函数内仅仅赋值，
		/// @param: 	const int ui_key_column_index - map的TKey所在列， 从0开始数
		///  @ret:		int
		///				0 - 成功
		///				其他 - 失败
		/// -------------------------------------------------------------------------------
		template<class TKey, class TValue>
		int table_data_map_(const QString str_table_name, std::map<TKey, TValue>* pout_map, const int ui_key_column_index) noexcept;

		template<class TKey, class TValue>
		int table_data_qmap_(const QString str_table_name, QMap<TKey, TValue>* pout_map, const int ui_key_column_index) noexcept;


		/// -------------------------------------------------------------------------------
		/// @brief:		只读取map_read_column_index列中的数据到pdata中
		/// @param: 	const QString str_table_name - 读取哪张表
		/// @param: 	std::map<TKey，TValue> * pout_map - 用于读取内容存放
		/// @param: 	const int ui_key_column_index - 主键再第几列， 从0开始数
		/// @param: 	const mapUIntUInt & map_read_column_index - 读取表格的哪些列， 从0开始数
		///  @ret:		int
		///				0 - 成功
		///				其他 - 失败
		/// -------------------------------------------------------------------------------
		template<class TKey, class TValue>
		int table_data_map_some_columns_(const QString str_table_name, std::map<TKey, TValue>* pout_map, const int ui_key_column_index, const MapUIntUInt& map_read_column_index) noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		读取不在map_read_column_index列中的数据到pdata中
		/// @param: 	const QString str_table_name - 读取哪张表
		/// @param: 	std::map<TKey，TValue> * pout_map - 用于读取内容存放
		/// @param: 	const int ui_key_column_index - 主键再第几列， 从0开始数
		/// @param: 	const mapUIntUInt & map_read_column_index - 不读取表格的哪些列， 从0开始数
		///  @ret:		int
		///				0 - 成功
		///				其他 - 失败
		/// -------------------------------------------------------------------------------
		template<class TKey, class TValue>
		int table_data_map_except_columns_(const QString str_table_name, std::map<TKey, TValue>* pout_map, const int ui_key_column_index, const MapUIntUInt& map_read_except_column_index) noexcept;




#ifdef BUG_CODE
		/// -------------------------------------------------------------------------------
		/// @brief:		将数据写入数据库，注意： pdata中的行数应该与数据库中表的数据相同，否则，将返回非0
		/// @param: 	const QString str_table_name - 哪张表
		/// @param: 	void * pdata - 带写入数据的起始地址
		/// @param: 	const uint key_index - 主键在哪一列,从0 开始数, 
		///  @ret:		int
		///				0 - 成功
		///				1 - 失败， 参数pdata为空
		///				2 - 失败， 无法获取对应表
		/// -------------------------------------------------------------------------------
		int update_table_(const QString str_table_name, void* pdata, const uint key_index) noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		只更新map_update_column_index中的列到数据库
		/// @param: 	const QString str_table_name - 更新哪张表
		/// @param: 	void * pdata - 待写入的数据
		/// @param: 	const uint key_index - 表格的主键再第几列，从0开始数
		/// @param: 	const mapUIntUInt map_update_column_index - 要写入那些列
		///  @ret:		int
		///				0 - 成功
		///				其他 - 失败
		/// -------------------------------------------------------------------------------
		int update_table_some_columns_(const QString str_table_name, void* pdata, const uint key_index, const MapUIntUInt map_update_column_index) noexcept;
#endif /// 
		/// -------------------------------------------------------------------------------
		/// @brief:		只更新不在map_update_column_index中的列到数据库
		/// @param: 	const QString str_table_name - 更新哪张表
		/// @param: 	void * pdata - 待写入的数据
		/// @param: 	const uint key_index - 表格的主键再第几列，从0开始数
		/// @param: 	const mapUIntUInt map_update_column_index - 不写入那些列
		///  @ret:		int
		///				0 - 成功
		///				其他 - 失败	
		/// -------------------------------------------------------------------------------
		int update_table_except_columns_(const QString str_table_name, void* pdata, const uint key_index, const MapUIntUInt map_except_column_index) noexcept;


		/// -------------------------------------------------------------------------------
		/// @brief:		将pmap_data中的数据写入[str_table_name]中, 注意： pmap_data中的行数应该与数据库中表的数据相同，否则，将返回6
		/// @param: 	const QString str_table_name - 对应书库中表的名字
		/// @param: 	const std::map<TKey, TValue> * pmap_value - 待写入数据内容
		/// @param: 	const uint key_column_index - str_table_name的主键在哪一列， 从0 开始数
		///  @ret:		int
		///				0 - 成功
		///				其他 - 失败
		/// -------------------------------------------------------------------------------
		template<typename TKey, typename TValue>
		int updateTableData(const QString str_table_name, const std::map<TKey, TValue>* pmap_data, const uint key_column_index) noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		将str_table_name表中的数据清空后，再重新写入
		/// @param: 	const QString str_table_name - 哪张表
		/// @param: 	const QMap<TKey， TValue> * pmap_data - 待写入的数据
		/// @param: 	const uint key_column_index - str_table_name 表对应的主键列索引
		///  @ret:		int
		///				
		/// -------------------------------------------------------------------------------
		template<typename TKey, typename TValue>
		int writeTableData(const QString str_table_name, const QMap<TKey, TValue>* pmap_data, const uint key_column_index) noexcept;


		/// -------------------------------------------------------------------------------
		/// @brief:		将map_update_column_index中的数据写入[str_table_name]中, 且只写入【map_update_column_index】中的列数据，
		/// @param: 	const QString str_table_name - 对应书库中表的名字
		/// @param: 	const std::map<TKey, TValue> * pmap_value - 待写入数据内容
		/// @param: 	const uint key_column_index - str_table_name的主键在哪一列， 从0 开始数
		/// @param: 	const mapUIntUInt map_update_column_index - 要写入哪些列的数据
		///  @ret:		int
		///				0 - 成功
		///				其他 - 失败
		/// -------------------------------------------------------------------------------
		template<typename TKey, typename TValue>
		int update_table_map_some_columns_(const QString str_table_name, const std::map<TKey, TValue>* pmap_data, const uint key_column_index, const MapUIntUInt& map_update_column_index) noexcept;


		/// -------------------------------------------------------------------------------
		/// @brief:		将不在map_except_column_index中的数据写入[str_table_name]中, 且只写入【map_update_column_index】中的列数据，
		/// @param: 	const QString str_table_name - 对应书库中表的名字
		/// @param: 	const std::map<TKey, TValue> * pmap_value - 待写入数据内容
		/// @param: 	const uint key_column_index - str_table_name的主键在哪一列， 从0 开始数
		/// @param: 	const mapUIntUInt map_update_column_index - 不要写入哪些列的数据
		///  @ret:		int
		///				0 - 成功
		///				其他 - 失败
		/// -------------------------------------------------------------------------------
		template<typename TKey, typename TValue>
		int update_table_map_except_columns_(const QString str_table_name, const std::map<TKey, TValue>* pmap_data, const uint key_column_index, const MapUIntUInt& map_except_column_index) noexcept;


		/// -------------------------------------------------------------------------------
		/// @brief:		检查文件后缀是否为可支持类型
		/// @param: 	const QString & str_suffix - 待检查的文件后缀
		///  @ret:		bool
		///				true - 支持， 
		///				false - 不支持
		/// -------------------------------------------------------------------------------
		bool file_suffix_is_right_(const QString& str_suffix) noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		将支持的文件以字符串的形式返回
		///  @ret:		QT_NAMESPACE::QString
		///				
		/// -------------------------------------------------------------------------------
		QString dbSuffix() noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		支持的数据库后缀
		///  @ret:		QT_NAMESPACE::QStringList
		///				
		/// -------------------------------------------------------------------------------
		QStringList dbSuffixList() noexcept;

	private:


		/// -------------------------------------------------------------------------------
		/// @brief:		初始化
		/// @param: 	const stSQLInit & st_init - 
		///  @ret:		int
		///				0 - 成功
		///				1 - 失败， st_init的第一个参数错误， 文件不存在
		///				2 - 失败， st_init的第二个参数错误， 不在给定的值中
		///				3 - 失败， st_init的第三个参数错误， 不在给定的值中
		///				
		/// -------------------------------------------------------------------------------
		int preInit(const stSQLInit& st_init) noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		初始化数据库驱动
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void initDataBase() noexcept;


		/// -------------------------------------------------------------------------------
		/// @brief:		解析数据库默认数据类型
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void init_analysis_db_data_type_() noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		构造函数中调用
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void initOther() noexcept;


		/// -------------------------------------------------------------------------------
		/// @brief:		将单元格中的数据放入对应的处理函数
		/// @param: 	void * pdata - 
		/// @param: 	const QVariant & var_data - 
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void* analysis_cell_value_int_(void* pdata, const QVariant& var_data, const bool is_move_data_ptr) noexcept;
		void* analysis_cell_value_str_(void* pdata, const QVariant& var_data, const bool is_move_data_ptr) noexcept;
		void* analysis_cell_value_double_(void* pdata, const QVariant& var_data, const bool is_move_data_ptr) noexcept;
		void* analysis_cell_value_bool_(void* pdata, const QVariant& var_data, const bool is_move_data_ptr) noexcept;
		void* analysis_cell_value_ushort_(void* pdata, const QVariant& var_data, const bool is_move_data_ptr) noexcept;
		void* analysis_cell_value_short_(void* pdata, const QVariant& var_data, const bool is_move_data_ptr) noexcept;
		void* analysis_cell_value_uint_(void* pdata, const QVariant& var_data, const bool is_move_data_ptr) noexcept;


		QSqlTableModel* table_model_(const QString& str_table_name) noexcept;


		/// -------------------------------------------------------------------------------
		/// @brief:		获取列名
		/// @param: 	const QString & str_table_name - 哪张表
		///  @ret:		QT_NAMESPACE::QStringList
		///				
		/// -------------------------------------------------------------------------------
		mapTableColumn table_column_type_(QSqlQueryModel* pModel, const QString& str_table_name) noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		读取表的行数
		/// @param: 	QSqlQueryModel * pModel - 
		/// @param: 	const QString & str_table_name - 
		///  @ret:		int
		///				
		/// -------------------------------------------------------------------------------
		int tableRowCount(QSqlQueryModel* pModel, const QString& str_table_name) noexcept;

		/// -------------------------------------------------------------------------------
		/// @brief:		删除参数中表格中的数据
		/// @param: 	QSqlQueryModel * pModel - 
		/// @param: 	const QString & str_table_name - 
		///  @ret:		int
		///				
		/// -------------------------------------------------------------------------------
		int removeTableAllRows(QSqlQueryModel* pModel, const QString& str_table_name) noexcept;


		/// -------------------------------------------------------------------------------
		/// @brief:		根据参数， 得到对应sql语句对应的部分
		/// @param: 	const QString & str_key_type - 当前数据是那种类型： TEXT 还是INTEGER
		/// @param: 	void * pdata - 对应的数据
		/// @param: 	QString * pout_value - 解析结果
		///  @ret:		void *
		///				数据区的下一个首地址
		/// -------------------------------------------------------------------------------
		void* column_sql_str_(const QString& str_key_type, void* pdata, QString* pout_value, bool& is_ok, const bool is_move_data_ptr) noexcept;



		/// -------------------------------------------------------------------------------
		/// @brief:		
		/// @param: 	QString str_key_type - 
		/// @param: 	void * pdata - 
		/// @param: 	QString * pout_value - 
		///  @ret:		void *
		///				
		/// -------------------------------------------------------------------------------
		void* column_sql_str_text_(void* pdata, QString* pout_value, const bool is_move_pdata) noexcept;
		void* column_sql_str_integer_(void* pdata, QString* pout_value, const bool is_move_pdata) noexcept;
		void* column_sql_str_double_(void* pdata, QString* pout_value, const bool is_move_pdata) noexcept;
		void* column_sql_str_boolean_(void* pdata, QString* pout_value, const bool is_move_pdata) noexcept;
		void* column_sql_str_uint_(void* pdata, QString* pout_value, const bool is_move_pdata) noexcept;
		void* column_sql_str_ushort_(void* pdata, QString* pout_value, const bool is_move_pdata) noexcept;
		void* column_sql_str_short_(void* pdata, QString* pout_value, const bool is_move_pdata) noexcept;


		/// 用作定义将任意类型转为指定的数据类型
		template<typename TDataType, typename TDestType>
		TDataType* TColumn_sql_str_(TDataType* pdata, QString* pout_value, const bool is_move_pdata) noexcept;

	private:
		/// 数据库连链接名称
		QString		str_data_base_connect_name_;
		/// 数据库操作对象
		QSqlDatabase		sql_data_base_;
		/// 保存对应数据库中的表
		MapTableModelView		map_db_table_;

		using mapStrDataType = std::map<QString, stTypeTransferFunc>;/// QMap<QString, stTypeTransferFunc>;
		/// <解析数据类型, 解析数据类型>
		mapStrDataType			map_analysis_db_type_;
		/// 保存当前支持的sqlite3的数据库类型
		HashSqliteSuffix		hash_sql_file_suffix_;

	};





}

#endif /// SQLITE_EX_H_