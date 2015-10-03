/*
 * buffer_module.cpp
 *
 *  Created on: Oct 3, 2015
 *      Author: loganek
 */


#include "buffer_module.h"

#include "controller/controller.h"

BufferModule::BufferModule()
: BaseMainModule(GstDebugger::GStreamerData::kBufferInfo, "buffers")
{
}

void BufferModule::load_details(gpointer data)
{
	auto buffer_info = (GstDebugger::BufferInfo*)data;

	append_details_row("Pts", std::to_string(buffer_info->pts()));
	append_details_row("Dts", std::to_string(buffer_info->dts()));
	append_details_row("Duration", std::to_string(buffer_info->duration()));
	append_details_row("Offset", std::to_string(buffer_info->offset()));
	append_details_row("Offset End", std::to_string(buffer_info->offset_end()));
	append_details_row("Data Size", std::to_string(buffer_info->size()));
	append_details_row("Object path", buffer_info->pad());

	if (buffer_info->has_data())
	{
		append_details_row("Data", "data"); // todo
	}
}

void BufferModule::data_received(const Gtk::TreeModel::Row& row, GstDebugger::GStreamerData *data)
{
	row[columns.header] = "Buffer of size " + std::to_string(data->buffer_info().size());
	row[columns.data] = new GstDebugger::BufferInfo(data->buffer_info());
}

BufferControlModule::BufferControlModule()
: ControlModule()
{
	pad_path_label = Gtk::manage(new Gtk::Label());
	main_box->pack_start(*pad_path_label, false, true);
	main_box->reorder_child(*pad_path_label, 0);

	data_check_button = Gtk::manage(new Gtk::CheckButton());
	main_box->pack_start(*data_check_button, false, true);
	main_box->reorder_child(*data_check_button, 1);

	create_dispatcher("selected-object", [this] {
		auto pad_path = controller->get_selected_pad_path();
		if (pad_path.empty())
			pad_path = "none (any path)";
		pad_path_label->set_text(pad_path);
	}, nullptr);

	hooks_tree_view->append_column("Pad", hooks_model_columns.str1);
}

void BufferControlModule::add_watch()
{
	controller->send_buffer_request_command(true, controller->get_selected_pad_path(),
			data_check_button->get_active());
}

void BufferControlModule::remove_watch(const Gtk::TreeModel::Row& row)
{
	Glib::ustring pad_path = row[hooks_model_columns.str1];
	controller->send_buffer_request_command(false, pad_path,
			data_check_button->get_active());
}

void BufferControlModule::confirmation_received(GstDebugger::Command* cmd)
{
	if (!cmd->has_pad_watch() || !cmd->pad_watch().has_buffer())
		return;

	auto confirmation = cmd->pad_watch();
	if (confirmation.action() == GstDebugger::ADD)
	{
		Gtk::TreeModel::Row row = *(hooks_model->append());
		row[hooks_model_columns.str1] = confirmation.pad();
	}
	else
	{
		remove_hook(confirmation);
	}
}