#pragma once
#include <QtWidgets>

#include "StudyPlan.h"

class FileMenu : public QMenu {
 public:
  FileMenu(QWidget *fileDialogParent, StudyPlan *studyPlan);
};