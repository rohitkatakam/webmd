#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Disease {
    string name = "";
    int frequency = 0;
    vector<string> symptoms;
    void addFrequency(int freq) {
        frequency = freq;
    }
};

vector<string> gatherSymptoms();

vector<Disease> convertCSV(string filename);

vector<Disease> checkSymptoms(vector<string> user, vector<Disease> bank);

vector<pair<string, double>> findLikelihood(vector<Disease> data);

void printResults(vector<pair<string, double>> final);

int main() {
    vector<string> userSymptoms;
    userSymptoms = gatherSymptoms();

    vector<Disease> possibleDiseases;
    possibleDiseases = convertCSV("/Users/rohitkatakam/projects/BummyWebMD 5.13.56 PM/mainRearranged.csv");

    vector<Disease> possible = checkSymptoms(userSymptoms, possibleDiseases);

    vector<pair<string, double>> likelihoods;
    likelihoods = findLikelihood(possible);

    printResults(likelihoods);
    return 0;
}

//gathers input from the user constructs a vector of symptoms
vector<string> gatherSymptoms() {
    string symptom;
    cout << "symptoms: ";
    getline(cin, symptom);
    vector<string> symptoms;
    while (symptom.compare("done") != 0) {
        symptoms.push_back(symptom);
        getline(cin, symptom);
    }
    return symptoms;
}

//take the dataset and fills a vector with diseases
vector<Disease> convertCSV(string filename) {
    //first, create a vector of all possible symptoms
    //while in a row, set newdisease name and frequency
    //if cell == 1, push back symptoms[(i % 409) -2]
    vector<Disease> dataset;
    vector<string> allSymptoms;
    fstream allDiseases;
    allDiseases.open(filename, ios::in);
    string full;
    string ID;
    Disease newdisease;
    string diseaseName;
    int index;
    //number of total cells: 55215
    //number of symptoms: 407
    for (int i = 0; i < 55215; i++) {
        //set full as the next cell
        getline(allDiseases, full, ',');
        //skip "label" and "frequency" cells
        if (i < 2) {
            continue;
        }
        if ((i % 408) > 2 && i > 410) {
            index = (i % 408) - 3;
            if (stoi(full) == 1) {
                dataset.back().symptoms.push_back(allSymptoms[index]);
            }
            continue;
        }
        if (i < 409) {
            if (i == 39) {
                continue;
            }
            //add symptom to bank of symptoms only until end of first row
            allSymptoms.push_back(full);
        }
            //might need to make this just a part of every loop
        else {
            if ((i % 408) == 1) {
                newdisease.name.clear();
                newdisease.name = full;
                dataset.push_back(newdisease);
            }
            else if ((i % 408) == 2) {
                dataset.back().addFrequency(stoi(full));
            }
        }
    }
    //this kinda works
    /*for (int i = 0; i < 410; i++) {
        getline(allDiseases, full, ',');
    }*/
    dataset.pop_back();
    allDiseases.close();
    return dataset;
    //i think this works now
}

//compare the user symptoms to each disease and if they are not all matches, get rid of that disease
vector<Disease> checkSymptoms(vector<string> user, vector<Disease> bank) {
    vector<Disease> final;
    //for every disease in the bank, add that disease to final if all the symptoms are present
    sort(user.begin(), user.end());
    //use this this shi gas kinda
    //i lied
    int total = 0;
    for (Disease disease : bank) {
        total = 0;
        //for every symptom except for done
        for (string userSymptom : user) {
            for (string diseaseSymptom : disease.symptoms) {
                if (userSymptom.compare(diseaseSymptom) == 0) {
                    total++;
                    break;
                }
            }
        }
        if (total == user.size()) {
            final.push_back(disease);
        }
    }
    return final;
}

//create a vector of pairs representing the name and percent likelihood of remaining diseases
vector<pair<string, double>> findLikelihood(vector<Disease> data) {
    vector<pair<string, double>> likelihoods;
    pair<string, double> entry;
    double totalFrequency = 0;
    double percent;
    for (Disease d : data) {
        totalFrequency += d.frequency;
    }
    for (Disease d : data) {
        entry.first = d.name;
        percent = d.frequency / totalFrequency;
        entry.second = percent;
        likelihoods.push_back(entry);
    }
    return likelihoods;
}

//print to terminal the results
void printResults(vector<pair<string, double>> final) {
    string printable;
    for (pair<string, double> p : final) {
        printable = p.first.substr(16, p.first.size() - 16);
        cout << printable << ": " << p.second * 100 << "%\n";
    }
}