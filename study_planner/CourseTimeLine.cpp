#include "CourseTimeLine.h"

#include <QDebug>
#include <string>

#include "CourseCard.h"

using namespace std;

CourseTimeLine::CourseTimeLine(const StudyPlan *studyPlan)
    : studyPlan(studyPlan), compactModeOn(false) {
  const unsigned int minColWidth = 100;
  for (unsigned int year = 1; year <= 4; year++) {
    for (int i = PERIOD1; i <= SUMMER2; i++) {
      Period period = (Period)i;
      unsigned int column = (year - 1) * 8 + period;
      string yearTitle =
          string("Year ") + to_string(period <= 3 ? year + 2024 : year + 2025);
      string periodTitle = periodToString(period);
      QLabel *yearLabel = new QLabel(yearTitle.c_str());
      QLabel *periodLabel = new QLabel(periodTitle.c_str());
      yearLabel->setFixedHeight(10);
      periodLabel->setFixedHeight(10);
      this->setColumnMinimumWidth(column, minColWidth);
      this->setColumnStretch(column, 1);
      this->addWidget(yearLabel, 0, column);
      this->addWidget(periodLabel, 1, column);
    }
  }
  this->setAlignment(Qt::AlignTop);
  fitCourses();
}

void CourseTimeLine::compactModeStateChanged(Qt::CheckState checkState) {
  switch (checkState) {
    case Qt::Unchecked:
      this->compactModeOn = false;
      emit toggleCompactMode(false);
      break;
    default:
      this->compactModeOn = true;
      emit toggleCompactMode(true);
  }
  qDebug() << compactModeOn;
}

void CourseTimeLine::fitCourses() {
  int count = this->count();
  int k = 0;
  for (int row = 2; row < this->rowCount(); row++)
    for (int column = 0; column < this->columnCount() && k < count; column++)
      if (this->itemAtPosition(row, column) != nullptr) {
        delete this->itemAtPosition(row, column)->widget();
        k++;
      };
  for (unsigned int year = 1; year <= 4; year++) {
    for (int i = PERIOD1; i <= SUMMER2; i++) {
      Period period = (Period)i;
      unsigned int column = (year - 1) * 8 + period;
      const double creditsForThisPeriod =
          studyPlan->getCreditsForPeriod(year, period);
      QLabel *creditsLabel = new QLabel(
          (to_string(creditsForThisPeriod)
               .substr(0, to_string(creditsForThisPeriod).find(".") + 3) +
           " credits")
              .c_str());
      this->addWidget(creditsLabel, 2, column);
    }
  }
  vector<shared_ptr<CoursePlan>> coursePlans =
      this->studyPlan->getCoursePlans();

  for (vector<shared_ptr<CoursePlan>>::iterator i = coursePlans.begin();
       i != coursePlans.end(); i++) {
    shared_ptr<CoursePlan> coursePlan = *i;
    CourseCard *courseCard = new CourseCard(coursePlan, this->compactModeOn);
    QObject::connect(courseCard, &CourseCard::deleteCoursePlan, this->studyPlan,
                     &StudyPlan::deleteCoursePlan);
    QObject::connect(this, &CourseTimeLine::toggleCompactMode, courseCard,
                     &CourseCard::toggleCompactMode);
    int row = 2;
    bool rowHasSpace = true;
    do {
      row++;
      rowHasSpace = true;
      for (int j = 0; j < coursePlan->getPlanLength(); j++) {
        rowHasSpace =
            rowHasSpace &&
            this->itemAtPosition(row, courseCard->column() + j) == nullptr;
      }
    } while (!rowHasSpace);
    this->addWidget(courseCard, row, courseCard->column(), 1,
                    coursePlan->getPlanLength());
  }
}
