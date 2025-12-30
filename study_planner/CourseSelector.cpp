#include "CourseSelector.h"

#include <QDebug>

using namespace std;

void CourseSelector::updateCourseList() {
  QLayoutItem *child;
  while ((child = this->courseList->takeAt(0)) != nullptr) {
    delete child->widget();
    delete child;
  }
  selectedCourseCard = nullptr;
  addCourseButton->setEnabled(false);

  for (vector<Course>::const_iterator i = this->courses.begin();
       i != this->courses.end(); i++) {
    if ((this->startPeriodFilter != 0 &&
         !(i->startsInPeriod(this->startPeriodFilter))) ||
        (this->examOnlyFilter && !(i->examOnlyPossible)) ||
        (this->examOnlyFilter && this->examPeriodFilter != 0 &&
         !(i->examInPeriod(this->examPeriodFilter))) ||
        (this->lengthFilter != 0 && i->length != this->lengthFilter) ||
        (this->nameOrCodeOrTagsFilter != "" &&
         !(i->nameOrCodeOrTagsContains(this->nameOrCodeOrTagsFilter))) ||
        this->studyPlan->courseInPlans(*i)) {
      continue;
    }
    CourseSelectCard *courseSelectCard = new CourseSelectCard(*i);
    this->courseList->addWidget(courseSelectCard);
    QObject::connect(
        courseSelectCard, &CourseSelectCard::clicked, this, [=]() -> void {
          if (this->selectedCourseCard != nullptr) {
            this->selectedCourseCard->setProperty("selected", false);
            this->selectedCourseCard->style()->unpolish(selectedCourseCard);
            this->selectedCourseCard->style()->polish(selectedCourseCard);
            this->selectedCourseCard->update();
          }
          this->selectedCourseCard = courseSelectCard;
          this->selectedCourseCard->setProperty("selected", true);
          this->selectedCourseCard->style()->unpolish(selectedCourseCard);
          this->selectedCourseCard->style()->polish(selectedCourseCard);
          this->selectedCourseCard->update();
          this->addCourseButton->setEnabled(true);
        });
  }
}

CourseSelector::CourseSelector(const std::vector<Course> &courses,
                               const StudyPlan *studyPlan)
    : selectedCourseCard(nullptr),
      startPeriodFilter((Period)0),
      lengthFilter(0),
      examOnlyFilter(false),
      examPeriodFilter((Period)0),
      nameOrCodeOrTagsFilter(""),
      studyPlan(studyPlan),
      courses(courses) {
  QLabel *startPeriodFilterDropDownLabel = new QLabel("Filter by start period");
  QComboBox *startPeriodFilterDropDown = new QComboBox();
  startPeriodFilterDropDown->addItem("All periods", 0);
  const vector<Period> allPeriods = {PERIOD1, PERIOD2, CHRISTMAS, PERIOD3,
                                     PERIOD4, PERIOD5, SUMMER1,   SUMMER2};
  addPeriodsToDropDown(allPeriods, startPeriodFilterDropDown);

  QLabel *lengthFilterDropDownLabel = new QLabel("Filter by length");
  QComboBox *lengthFilterDropDown = new QComboBox();
  lengthFilterDropDown->addItem("All lengths", 0);
  for (int i = 1; i <= 8; i++) {
    lengthFilterDropDown->addItem((to_string(i) + " periods").c_str(), i);
  }

  QCheckBox *examOnlyFilterCheckbox =
      new QCheckBox("Show only exam only courses");

  QLabel *examPeriodFilterDropDownLabel = new QLabel("Filter by exam period");
  QComboBox *examPeriodFilterDropDown = new QComboBox();
  examPeriodFilterDropDown->addItem("All periods", 0);
  addPeriodsToDropDown(allPeriods, examPeriodFilterDropDown);
  examPeriodFilterDropDownLabel->hide();
  examPeriodFilterDropDown->hide();

  QLabel *courseSearchLabel = new QLabel("Search by name, code or tags");
  QLineEdit *courseSearchLineEdit = new QLineEdit();
  QPushButton *courseSearchButton = new QPushButton("search");
  courseSearchButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  courseSearchLineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

  QScrollArea *courseListScrollArea = new QScrollArea();
  QWidget *courseListWidget = new QWidget();
  this->courseList = new QVBoxLayout();
  courseListWidget->setLayout(this->courseList);
  courseListScrollArea->setWidget(courseListWidget);
  courseListScrollArea->setWidgetResizable(true);
  courseListScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  courseListScrollArea->setSizePolicy(QSizePolicy::Fixed,
                                      QSizePolicy::Expanding);
  this->addCourseButton = new QPushButton("add course");
  this->addCourseButton->setEnabled(false);
  this->addCourseButton->setSizePolicy(QSizePolicy::Minimum,
                                       QSizePolicy::Fixed);

  this->updateCourseList();
  connect(startPeriodFilterDropDown, &QComboBox::currentIndexChanged, this,
          [=]() -> void {
            this->startPeriodFilter =
                startPeriodFilterDropDown->currentData().value<Period>();
            this->updateCourseList();
          });
  connect(examOnlyFilterCheckbox, &QCheckBox::checkStateChanged, this,
          [=]() -> void {
            if (examOnlyFilterCheckbox->isChecked()) {
              examPeriodFilterDropDownLabel->show();
              examPeriodFilterDropDown->show();
            } else {
              examPeriodFilterDropDownLabel->hide();
              examPeriodFilterDropDown->hide();
            }
            this->examOnlyFilter = examOnlyFilterCheckbox->isChecked();
            this->updateCourseList();
          });
  connect(examPeriodFilterDropDown, &QComboBox::currentIndexChanged, this,
          [=]() -> void {
            this->examPeriodFilter =
                examPeriodFilterDropDown->currentData().value<Period>();
            this->updateCourseList();
          });
  connect(courseSearchButton, &QPushButton::clicked, this, [=]() -> void {
    this->nameOrCodeOrTagsFilter =
        string(courseSearchLineEdit->text().toUtf8());
    this->updateCourseList();
  });
  connect(lengthFilterDropDown, &QComboBox::currentIndexChanged, this,
          [=]() -> void {
            this->lengthFilter = lengthFilterDropDown->currentData().toUInt();
            this->updateCourseList();
          });
  connect(addCourseButton, &QPushButton::clicked, this, [=]() -> void {
    emit selectCourse(this->selectedCourseCard->getCourse());
  });

  this->addWidget(startPeriodFilterDropDownLabel);
  this->addWidget(startPeriodFilterDropDown);
  this->addWidget(lengthFilterDropDownLabel);
  this->addWidget(lengthFilterDropDown);
  this->addWidget(examOnlyFilterCheckbox);
  this->addWidget(examPeriodFilterDropDownLabel);
  this->addWidget(examPeriodFilterDropDown);
  this->addWidget(courseSearchLabel);
  this->addWidget(courseSearchLineEdit);
  this->addWidget(courseSearchButton);
  this->addWidget(courseListScrollArea);
  this->addWidget(addCourseButton);
  this->setAlignment(Qt::AlignTop);
}