#include "StudyPlan.h"

#include <QDebug>
#include <fstream>
#include <nlohmann/json.hpp>

#include "ExamPlan.h"
#include "ParticipationPlan.h"

using namespace std;
using json = nlohmann::json;

void StudyPlan::startDialog(const Course& course) {
  CoursePlanDialog* coursePlanDialog = new CoursePlanDialog(course);
  QObject::connect(coursePlanDialog, &CoursePlanDialog::addCoursePlan, this,
                   &StudyPlan::addCoursePlan);
  coursePlanDialog->open();
}

double StudyPlan::getCreditsForPeriod(unsigned int year, Period period) const {
  double sum = 0.0;
  for (std::vector<std::shared_ptr<CoursePlan>>::const_iterator i =
           coursePlans.begin();
       i != coursePlans.end(); i++) {
    const Period startPeriod = (*i)->getStartPeriod();
    const unsigned int yearOfStudies = (*i)->getYearOfStudies();
    Period endPeriod = startPeriod;
    for (int j = 0; j < (*i)->getPlanLength(); j++) {
      cyclePeriod(endPeriod);
    }
    if ((endPeriod > startPeriod && startPeriod <= period &&
         period < endPeriod && yearOfStudies == year) ||
        (endPeriod <= startPeriod &&
         (startPeriod <= period && yearOfStudies == year ||
          period < endPeriod && yearOfStudies + 1 == year))) {
      sum += ((*i)->getCourseCredits() * 1.0) / (*i)->getPlanLength();
    }
  }
  return sum;
}

unsigned int StudyPlan::getTotalCredits() {
  unsigned int sum = 0;
  for (vector<shared_ptr<CoursePlan>>::const_iterator i =
           this->coursePlans.begin();
       i != this->coursePlans.end(); i++) {
    sum += (*i)->getCourseCredits();
  }
  return sum;
}

bool StudyPlan::courseInPlans(const Course& course) const {
  for (vector<shared_ptr<CoursePlan>>::const_iterator i =
           this->coursePlans.begin();
       i != this->coursePlans.end(); i++) {
    if ((*i)->getCourseCode() == course.code) return true;
  }
  return false;
}

void StudyPlan::addCoursePlan(shared_ptr<CoursePlan> coursePlan) {
  this->coursePlans.push_back(coursePlan);
  emit studyPlanChanged();
}

void StudyPlan::deleteCoursePlan(shared_ptr<CoursePlan> coursePlan) {
  std::vector<shared_ptr<CoursePlan>>::iterator coursePlanToDelete =
      find(this->coursePlans.begin(), this->coursePlans.end(), coursePlan);
  if (coursePlanToDelete != this->coursePlans.end()) {
    this->coursePlans.erase(coursePlanToDelete);
  }
  emit studyPlanChanged();
}

void StudyPlan::loadStudyPlan(QWidget* fileDialogParent) {
  QString filename = QFileDialog::getOpenFileName(
      fileDialogParent, "Open file", "C:/", "JSON files (*.json)", nullptr,
      QFileDialog::DontUseNativeDialog);
  if (filename == "") {
    return;
  }
  fstream studyPlanFileStream(filename.toUtf8(), fstream::in);
  if (!studyPlanFileStream) {
    return;
  }
  json studyPlanData;
  studyPlanFileStream >> studyPlanData;
  vector<json> coursePlanJsons = studyPlanData;
  this->coursePlans = vector<shared_ptr<CoursePlan>>();
  for (vector<json>::const_iterator i = coursePlanJsons.begin();
       i != coursePlanJsons.end(); i++) {
    for (vector<Course>::const_iterator availableCourse =
             availableCourses.begin();
         availableCourse != availableCourses.end(); availableCourse++) {
      string courseCode = (*i)["courseCode"];
      if (availableCourse->code == courseCode) {
        const Course& courseToAdd = *availableCourse;
        if ((*i)["examPlan"]) {
          shared_ptr<CoursePlan> coursePlanToAdd =
              shared_ptr<ExamPlan>(new ExamPlan(
                  courseToAdd, (*i)["length"], (*i)["startPeriod"],
                  (*i)["examPeriod"], (*i)["yearOfStudies"], (*i)["examYear"]));
          this->coursePlans.push_back(coursePlanToAdd);
        } else {
          shared_ptr<CoursePlan> coursePlanToAdd =
              shared_ptr<ParticipationPlan>(new ParticipationPlan(
                  courseToAdd, (*i)["startPeriod"], (*i)["yearOfStudies"]));
          this->coursePlans.push_back(coursePlanToAdd);
        }
      }
    }
  }
  studyPlanFileStream.close();
  emit studyPlanChanged();
}

void StudyPlan::saveStudyPlan(QWidget* fileDialogParent) {
  QFileDialog* saveFileDialog =
      new QFileDialog(fileDialogParent, "Save file", "C:/");
  saveFileDialog->setOption(QFileDialog::DontUseNativeDialog);
  saveFileDialog->setFileMode(QFileDialog::AnyFile);
  saveFileDialog->setLabelText(QFileDialog::Accept, "Save");
  saveFileDialog->exec();
  QStringList selectedFiles = saveFileDialog->selectedFiles();
  if (selectedFiles[0] == "") {
    return;
  }
  fstream studyPlanFileStream(selectedFiles[0].toUtf8(), fstream::out);
  if (!studyPlanFileStream) {
    return;
  }
  json studyPlanToSave = json::array();
  for (std::vector<std::shared_ptr<CoursePlan>>::const_iterator i =
           this->coursePlans.begin();
       i != coursePlans.end(); i++) {
    json coursePlanToSave = json::object();
    coursePlanToSave["courseCode"] = (*i)->getCourseCode();
    coursePlanToSave["startPeriod"] = (*i)->getStartPeriod();
    coursePlanToSave["yearOfStudies"] = (*i)->getYearOfStudies();
    if ((*i)->isExamPlan()) {
      coursePlanToSave["examPlan"] = true;
      coursePlanToSave["length"] = (*i)->getPlanLength();
      coursePlanToSave["examPeriod"] = (*i)->getExamPeriod();
      coursePlanToSave["examYear"] = (*i)->getExamYear();
    } else {
      coursePlanToSave["examPlan"] = false;
    }
    studyPlanToSave.push_back(coursePlanToSave);
  }
  studyPlanFileStream << studyPlanToSave;
  studyPlanFileStream.close();
}
