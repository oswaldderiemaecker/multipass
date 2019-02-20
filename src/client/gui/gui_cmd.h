/*
 * Copyright (C) 2019 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MULTIPASS_GUI_CMD_H
#define MULTIPASS_GUI_CMD_H

#include <multipass/cli/command.h>

#include <QFutureSynchronizer>
#include <QFutureWatcher>
#include <QMenu>
#include <QObject>
#include <QSystemTrayIcon>
#include <QTimer>

#include <memory>
#include <unordered_map>
#include <vector>

namespace multipass
{
class ArgParser;

namespace cmd
{
class GuiCmd final : public QObject, public Command
{
    Q_OBJECT
public:
    using Command::Command;
    ReturnCode run(ArgParser* parser) override;

    std::string name() const override
    {
        return "";
    };

    QString short_help() const override
    {
        return "";
    };

    QString description() const override
    {
        return "";
    };

private:
    ParseCode parse_args(ArgParser* parser) override
    {
        return ParseCode::Ok;
    };

    void create_actions();
    void create_menu();
    void update_menu();
    void initiate_menu_layout();
    ListReply retrieve_all_instances();
    void set_menu_actions_for(const std::string& instance_name, const QString& state);
    void start_instance_for(const std::string& instance_name);
    void suspend_instance_for(const std::string& instance_name);
    void stop_instance_for(const std::string& instance_name);

    QSystemTrayIcon tray_icon;
    QMenu tray_icon_menu;

    QAction* retrieving_action;
    QAction* about_separator;
    QAction* about_action;
    QAction* quit_action;
    QAction failure_action{"Failure retrieving instances"};

    struct InstanceEntry
    {
        QString state;
        std::unique_ptr<QMenu> menu;
    };
    std::unordered_map<std::string, InstanceEntry> instances_entries;

    QFuture<ListReply> list_future;
    QFutureWatcher<ListReply> list_watcher;
    QFutureSynchronizer<void> future_synchronizer;

    QTimer menu_update_timer;
};
} // namespace cmd
} // namespace multipass
#endif // MULTIPASS_GUI_CMD_H
