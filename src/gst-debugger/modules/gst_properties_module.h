/*
 * gst_properties_module.h
 *
 *  Created on: Jul 14, 2015
 *      Author: loganek
 */

#ifndef SRC_GST_DEBUGGER_GST_PROPERTIES_MODULE_H_
#define SRC_GST_DEBUGGER_GST_PROPERTIES_MODULE_H_

#include "controller/iview.h"
#include "gvalue-converter/gvalue_base.h"

#include "gstdebugger.pb.h"

#include <gtkmm.h>

class GstPropertiesModule : public IBaseView
{
private:
	Gtk::Button *show_properties_button;
	Gtk::Box *properties_box;

	void showPropertiesButton_clicked_cb();

	std::string previous_element_path;

	void append_property(const std::shared_ptr<GValueBase>& value_base, const std::string &prop_name);
	bool update_property(const std::shared_ptr<GValueBase>& value_base, const std::string prop_name);
	void request_selected_element_property(const std::string &property_name);

	void show_pad_properties();

	void new_property(const GstDebugger::PropertyValue& property);
	void new_property_();

	void selected_object_changed();
	void selected_object_changed_();

	void clear_widgets();

public:
	GstPropertiesModule(const Glib::RefPtr<Gtk::Builder>& builder);

	void set_controller(const std::shared_ptr<Controller> &controller) override;
};

#endif /* SRC_GST_DEBUGGER_GST_PROPERTIES_MODULE_H_ */
