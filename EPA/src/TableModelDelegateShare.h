#pragma once
#include <QScopedPointer>
#include <QTableView>
#

class TableModelDelegateShare
{
public:

	template<typename TModel>
	void TableViewRowRemove(QTableView* ptv, TModel* pmodel)
	{
		if (nullptr == ptv || nullptr == pmodel)
		{
			return;
		}

		const int cur_row = ptv->currentIndex().row();
		if (-1 == cur_row)
		{
			return;
		}

		auto&& list_data = pmodel->modelData();
		list_data.removeAt(cur_row);
		pmodel->setModelData(list_data);

		/// 删除完成后吗设置自动选择行
		ptv->selectRow(cur_row);
	}


	template<typename TModel, typename TModelData>
	void TableViewRowRemove(TModel* pmodel, TModelData* list_origin_data, QList<int>& list_remove_rows)
	{
		if (nullptr == pmodel || nullptr == list_origin_data)
		{
			return;
		}
		/// 排序
		qSort(list_remove_rows.begin(), list_remove_rows.end());

		//const int& total_count = list_remove_rows.count();
		int del_index = 0;
		for (auto& item : list_remove_rows)
		{
			list_origin_data->removeAt(item - del_index);
			++del_index;
		}

		pmodel->setModelData(*list_origin_data);
	}


	template<typename TModel, typename TValue>
	void TableViewRowInsert(QTableView* ptv, TModel* pmodel)
	{
		if (nullptr == ptv || nullptr == pmodel)
		{
			return;
		}

		int cur_row = ptv->currentIndex().row();
		if (-1 == cur_row)
		{
			cur_row = pmodel->rowCount();
		}

		auto&& list_data = pmodel->modelData();
		TValue insert_item;
		list_data.insert(cur_row + 1, insert_item);
		pmodel->setModelData(list_data);

		ptv->selectRow(cur_row);
	}


	template<typename TModel, typename TItemDelegate, typename TValueType>
	void BaseInit(QTableView* ptv, QScopedPointer<TModel>& pmodel, QScopedPointer<TItemDelegate>& pdelegate, const TValueType vt)
	{
		if (nullptr == ptv)
		{
			return;
		}

		pmodel.reset(new(std::nothrow) TModel(vt, ptv));
		ptv->setModel(pmodel.data());

		pdelegate.reset(new(std::nothrow) TItemDelegate(vt, ptv));
		ptv->setItemDelegate(pdelegate.data());
	}

	template<typename TModel, typename TItemDelegate>
	void BaseInit(QTableView* ptv, QScopedPointer<TModel>& pmodel, QScopedPointer<TItemDelegate>& pdelegate)
	{
		if (nullptr == ptv)
		{
			return;
		}

		pmodel.reset(new(std::nothrow) TModel(ptv));
		ptv->setModel(pmodel.data());

		pdelegate.reset(new(std::nothrow) TItemDelegate(ptv));
		ptv->setItemDelegate(pdelegate.data());
	}


	template<typename TListType, typename TMapType>
	void updateUIDataToMem(TMapType& map_target, const TListType& list_data)
	{
		/// 清空
		{
			TMapType tmp;
			map_target.swap(tmp);
		}
		for (const auto& item : list_data)
		{
			map_target.insert(item.id_, item);
		}
	}
};

