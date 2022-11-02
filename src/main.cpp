#include <QApplication>

#include "win.h"

/*
    + добавление к структуре ReportExif название фотографии
    + добавление столбца - название фотографии
    + map<hex код, название параметра> 
    + добавление названий столбцов


    - фокус на кнопке "Анализировать"
    - сообщение 
        + о картинках, которые не содержат exif
        - о том, что в директории не нашлось фотографий jpeg
        - о том, что директории не существует
    - отображение в таблице не найденных данных (метка)
    - обработка русских букв в названии пути к директории
    
    - задание пути через File Browser
    - кэширование предыдущего запроса
    - гиперссылка на статью со всеми тегами
    - подсказки к лейблам 
    - ... 

    текущие задачи:
        - добавление выбора параметров 
            - основные через checkBox
            - другие через явное задание тега (с названием самого тега)


    - в одном потоке извлечение метаданных, во втором потоке progressBar
*/

/*
    Если GPS и в чекбоксе и в строке - в таблице повторяется (плохо)
    Не поддерживается русский в регулярном выражении
    Сообщения 
        - о том, что в директории не нашлось фотографий jpeg
        - о том, что директории не существует

*/


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Win w;
    w.show();
    return a.exec();
}
