#include "FileMenu.h"

FileMenu::FileMenu(QWidget *fileDialogParent, StudyPlan *studyPlan)
    : QMenu("File") {
  QAction *openFileAction = new QAction();
  openFileAction->setText("Open study plan");
  QObject::connect(
      openFileAction, &QAction::triggered, studyPlan,
      [=]() -> void { studyPlan->loadStudyPlan(fileDialogParent); });
  QAction *saveFileAction = new QAction();
  saveFileAction->setText("Save current plan");
  QObject::connect(
      saveFileAction, &QAction::triggered, studyPlan,
      [=]() -> void { studyPlan->saveStudyPlan(fileDialogParent); });
  this->addAction(openFileAction);
  this->addAction(saveFileAction);
};