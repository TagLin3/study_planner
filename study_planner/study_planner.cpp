#include <QDebug>
#include <QtWidgets>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

#include "CourseCard.h"
#include "CoursePlanDialog.h"
#include "CourseSelector.h"
#include "CourseTimeLine.h"
#include "ExamPlan.h"
#include "FileMenu.h"
#include "ParticipationPlan.h"
#include "StudyPlan.h"

using namespace std;
using json = nlohmann::json;

int main(int argc, char *argv[]) {
  fstream courseFileStream("../data/courses.json", fstream::in);
  if (!courseFileStream) {
    qDebug() << "Coursedata file courses.json not found.";
    return 1;
  }
  json courseData;
  courseFileStream >> courseData;
  vector<json> coursesInJson = courseData;
  vector<Course> availableCourses = vector<Course>();
  for (vector<json>::const_iterator i = coursesInJson.begin();
       i != coursesInJson.end(); i++) {
    json c = *i;
    Course courseToAdd =
        Course(c["code"], c["name"], c["credits"], c["length"],
               c["startPeriods"], c["examOnlyPossible"], c["examPeriods"],
               c["evenOrOddYears"], c["examEveryYear"], c["tags"]);
    availableCourses.push_back(courseToAdd);
  }
  courseFileStream.close();

  StudyPlan *studyPlan = new StudyPlan(availableCourses);

  QApplication app(argc, argv);
  QMainWindow mainWindow;
  QWidget *centralWidget = new QWidget();
  QMenu *fileMenu = new FileMenu(&mainWindow, studyPlan);
  mainWindow.menuBar()->addMenu(fileMenu);

  QWidget *timeLineWidget = new QWidget();
  CourseTimeLine *courseTimeLine = new CourseTimeLine(studyPlan);
  timeLineWidget->setLayout(courseTimeLine);
  QScrollArea *scrollableTimeLine = new QScrollArea();
  scrollableTimeLine->setWidget(timeLineWidget);
  scrollableTimeLine->setWidgetResizable(true);
  scrollableTimeLine->setSizePolicy(QSizePolicy::Expanding,
                                    QSizePolicy::Expanding);

  QHBoxLayout *bottomMenu = new QHBoxLayout();
  QLabel *totalPoints = new QLabel(
      ("Total study points: " + to_string(studyPlan->getTotalCredits()))
          .c_str());
  QCheckBox *compactModeCheckBox = new QCheckBox("compact mode");
  bottomMenu->addWidget(totalPoints);
  bottomMenu->addWidget(compactModeCheckBox);
  QObject::connect(compactModeCheckBox, &QCheckBox::checkStateChanged,
                   courseTimeLine, &CourseTimeLine::compactModeStateChanged);

  QVBoxLayout *studyPlanArea = new QVBoxLayout();
  studyPlanArea->addWidget(scrollableTimeLine);
  studyPlanArea->addLayout(bottomMenu);

  CourseSelector *courseSelector =
      new CourseSelector(availableCourses, studyPlan);
  QObject::connect(courseSelector, &CourseSelector::selectCourse, studyPlan,
                   &StudyPlan::startDialog);
  QObject::connect(studyPlan, &StudyPlan::studyPlanChanged, courseTimeLine,
                   &CourseTimeLine::fitCourses);
  QObject::connect(studyPlan, &StudyPlan::studyPlanChanged, totalPoints, [=]() {
    totalPoints->setText(
        ("Total study points: " + to_string(studyPlan->getTotalCredits()))
            .c_str());
  });
  QObject::connect(studyPlan, &StudyPlan::studyPlanChanged, courseSelector,
                   &CourseSelector::updateCourseList);

  mainWindow.resize(1000, 600);

  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addLayout(studyPlanArea);
  mainLayout->addLayout(courseSelector);

  centralWidget->setStyleSheet(
      "#courseCard { border: 2px solid black }"
      "#examCard { border: 2px solid blue }"
      "#courseSelectCard[selected=true] { border: 2px solid red }"
      "#courseSelectCard[selected=false] { border: 2px solid black }"
      "#courseSelectCard:hover { background-color: #222222 }");
  centralWidget->setLayout(mainLayout);
  mainWindow.setWindowTitle("study planner");
  mainWindow.setCentralWidget(centralWidget);
  mainWindow.show();

  return app.exec();
}