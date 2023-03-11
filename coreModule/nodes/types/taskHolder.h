#pragma once
#ifndef GENERIC_TASKHOLDER_H
#define GENERIC_TASKHOLDER_H

#include <deque>
#include <functional>

namespace generic::coreModule {

    enum class eTasksStatus {
        STATUS_OK = 0U,
        STATUS_ERROR_BREAK
    };
    enum class eTasksLoad {
        ON_ENTER = 0U,
        MANUAL
    };

    typedef std::function<eTasksStatus()> nodeTasks;

    class taskHolder {
    public:
        taskHolder();
        virtual ~taskHolder() = default;

        void executeTasks();
        eTasksLoad getTasksLoad() { return _loadTasks; }

    protected:
        /***
         * Loading nodes with tasks
         * @return std::function<bool()>
         */
        virtual std::deque<nodeTasks> getTasks() = 0;
        void proceed();

        void setTasksLoad(eTasksLoad val) { _loadTasks = val; }

    private:
        bool _allTasksLoaded = false;
        std::deque<nodeTasks> _allTasks;
        eTasksLoad _loadTasks = eTasksLoad::ON_ENTER;
    };
}// namespace generic::coreModule


#endif// GENERIC_TASKHOLDER_H
