#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

struct Question {
    int number;
    string text;
    vector<string> options;
};

// Función para comparar preguntas para ordenarlas
bool compareQuestions(const Question &q1, const Question &q2) {
    return q1.number < q2.number;
}

// Función para verificar si una línea es una opción de pregunta
bool isOption(const string &line) {
    if (line.empty()) {
        return false;
    }
    return islower(line[0]) && line[1] == ')';
}

int main() {
    ifstream inputFile("preguntas.txt");
    ofstream outputFile("preguntas_ordenadas.txt");

    if (!inputFile.is_open()) {
        cout << "Error al abrir el archivo de entrada." << endl;
        return 1;
    }

    if (!outputFile.is_open()) {
        cout << "Error al abrir el archivo de salida." << endl;
        return 1;
    }

    vector<Question> questions;
    set<string> questionTexts; // Para evitar preguntas repetidas
    int uniqueQuestionsCount = 0; // Contador de preguntas únicas
    int repeatedQuestionsCount = 0; // Contador de preguntas repetidas

    string line;
    int questionNumber = 1;
    bool isRepeatedQuestion = false; // Bandera para indicar si la pregunta es repetida

    while (getline(inputFile, line)) {
        if (line.empty()) {
            continue; // Ignorar líneas vacías
        }

        // Verificar si la línea es una opción
        if (isOption(line)) {
            // Si es una opción, agregarla a la última pregunta
            if (!questions.empty() && !isRepeatedQuestion) {
                questions.back().options.push_back(line);
            }
            continue;
        }

        Question q;

        // Leer el enunciado de la pregunta
        size_t pos = line.find('.'); // Encontrar la posición del primer punto
        if (pos != string::npos) {
            q.text = line.substr(pos + 1); // Tomar el enunciado de la pregunta después del primer punto
        } else {
            q.text = line; // Si no hay punto, tomar toda la línea como enunciado
        }

        // Verificar si la pregunta ya existe
        if (questionTexts.find(q.text) != questionTexts.end()) {
            // Si ya existe la pregunta, ignorarla y marcar como pregunta repetida
            cout << "Pregunta repetida: " << q.text << endl;
            isRepeatedQuestion = true;
            repeatedQuestionsCount++;
        } else {
            // Si es una pregunta única, agregarla al vector y aumentar el número de pregunta
            q.number = questionNumber++;
            questionTexts.insert(q.text);
            isRepeatedQuestion = false;
            questions.push_back(q);
            uniqueQuestionsCount++;
        }
    }

    int totalQuestions = uniqueQuestionsCount + repeatedQuestionsCount;
    double repeatedPercentage = (static_cast<double>(repeatedQuestionsCount) / totalQuestions) * 100;
    double uniquePercentage = (static_cast<double>(uniqueQuestionsCount) / totalQuestions) * 100;

    // Ordenar las preguntas por número
    sort(questions.begin(), questions.end(), compareQuestions);

    // Escribir las preguntas ordenadas en el archivo de salida
    for (const auto &q : questions) {
        outputFile << q.number << " - " << q.text << endl;
        for (const auto &option : q.options) {
            outputFile << option << endl;
        }
        outputFile << endl; // Agregar una línea en blanco entre preguntas
    }

    cout << endl << "Preguntas ordenadas y escritas en el archivo 'preguntas_ordenadas.txt'." << endl;
    cout << "  |--> Cantidad total de preguntas únicas: " << uniqueQuestionsCount << " --> " << uniquePercentage << "%" << endl;
    cout << "  |--> Cantidad de preguntas repetidas encontradas: " << repeatedQuestionsCount << " --> " << repeatedPercentage << "%" << endl;

    inputFile.close();
    outputFile.close();

    return 0;
}

