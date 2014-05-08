#include "names.h"

QHash<QString, unsigned int>* Names::params = new QHash<QString, unsigned int>();
QHash<QString, unsigned int>* Names::water_types = new QHash<QString, unsigned int>();
QHash<QString, unsigned int>* Names::locations = new QHash<QString, unsigned int>();
QHash<QString, unsigned int>* Names::item_types = new QHash<QString, unsigned int>();

unsigned int Names::analitic_id = 0;

Names::Names()
{
}
//Names::Names(const Names &other)
