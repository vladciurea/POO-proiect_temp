#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <limits>
# include <cstdlib>   // rand


class Player;
class Country;
class Ability;
class ResourceChest;
class Advisor;


class Country {
    std::string name;
    std::vector<Country*> neighbors; // pointeri catre tari vecine
    Player* owner = nullptr;
    int resourceProdIndex = 1;       // valoare de baza de productie
    int resourceTier = 1;            // tier (determina cost si productie)

public:
    Country(std::string n,
            const std::vector<Country *> &neigh,
            Player *own,
            int prodIndex,
            int tier)
        : name(std::move(n)), neighbors(neigh), owner(own),
          resourceProdIndex(prodIndex), resourceTier(tier) {}

    Country(const Country& other)
        : name(other.name),
          neighbors(other.neighbors),
          owner(other.owner),
          resourceProdIndex(other.resourceProdIndex),
          resourceTier(other.resourceTier) {
        std::cout << "[Country] Copy constructor pentru " << name << '\n';
    }

    Country& operator=(const Country& other) {
        if (this != &other) {
            name = other.name;
            neighbors = other.neighbors;
            owner = other.owner;
            resourceProdIndex = other.resourceProdIndex;
            resourceTier = other.resourceTier;
        }
        std::cout << "[Country] operator= pentru " << name << '\n';
        return *this;
    }

    ~Country() {
        std::cout << "[Country] Destructor pentru " << name << '\n';
    }

    [[nodiscard]] const std::string& getName() const { return name; }
    [[nodiscard]] int getTier() const { return resourceTier; }
    [[nodiscard]] Player* getOwner() const { return owner; }

    void setOwner(Player* p) { owner = p; }

    bool isNeighbor(const Country* c) const {
        return std::ranges::any_of(neighbors,
                                   [c](const Country* n){ return n == c; });
    }

    [[nodiscard]] int productionPerTurn() const {
        return resourceProdIndex * resourceTier * (10 + resourceTier);
    }

    [[nodiscard]] int costToBuy() const {
        return 50 * resourceTier + productionPerTurn() * 2;
    }

    friend std::ostream& operator<<(std::ostream& os, const Country& c) {
        os << "Country[" << c.name << "] tier=" << c.resourceTier
           << " prodIndex=" << c.resourceProdIndex
           << " owner=" << (c.owner ? "Player" : "None");
        return os;
    }
};


class Ability {
    std::string name;
    std::string description;
    int goldCost = 0;
    int stabilityEffect = 0;
    bool consumable = true;
public:
    Ability(std::string n, std::string d, int cost, int stabEff, bool cons = true)
        : name(std::move(n)), description(std::move(d)),
          goldCost(cost), stabilityEffect(stabEff), consumable(cons) {}

    [[nodiscard]] const std::string& getName() const { return name; }
    [[nodiscard]] int getGoldCost() const { return goldCost; }
    [[nodiscard]] int getStabilityEffect() const { return stabilityEffect; }
    [[nodiscard]] bool isConsumable() const { return consumable; }

    friend std::ostream& operator<<(std::ostream& os, const Ability& a) {
        os << "Ability[" << a.name << "] cost=" << a.goldCost
           << " stabEffect=" << a.stabilityEffect
           << " desc=\"" << a.description << "\"";
        return os;
    }
};


class ResourceChest {
    std::string question;
    std::vector<std::string> options;
    int correctIndex = 0;
    int rewardGold = 100;
    int lifeTimeTurns = 5;
public:
    ResourceChest(std::string q, const std::vector<std::string>& opts, int correct, int reward, int life)
        : question(std::move(q)), options(opts), correctIndex(correct),
          rewardGold(reward), lifeTimeTurns(life) {}

    [[nodiscard]] int presentAndResolve() const {
        std::cout << "---- Resource Chest ----\n";
        std::cout << question << "\n";
        for (size_t i = 0; i < options.size(); ++i) {
            std::cout << "  " << (i+1) << ") " << options[i] << "\n";
        }
        std::cout << "Alege optiunea (numar): ";
        int choice = 0;
        if(!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input invalid. Nu primesti aur.\n";
            return 0;
        }
        if (choice - 1 == correctIndex) {
            std::cout << "Raspuns corect! Primesti " << rewardGold << " aur.\n";
            return rewardGold;
        } else {
            std::cout << "Raspuns gresit. Nicio recompensa.\n";
            return 0;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const ResourceChest& rc) {
        os << "ResourceChest[q=\"" << rc.question << "\" reward=" << rc.rewardGold
           << " life=" << rc.lifeTimeTurns << "]";
        return os;
    }
};


class Advisor {
    std::string name;
    std::string specialty;   // ex: "Economic", "Military", "Diplomatic"
    int hireCost  = 0;       // costul de angajare (aur)
    int salaryCostPerTurn = 0; // salariu pe turn (aur)
    float loyalty = 80.0f;   // 0..100
    bool active = false;     // a fost angajat?
    int lastRecommendedAction = -1; // retinem ultima recomandare

public:
    Advisor(std::string n, std::string spec, int hire, int salary)
        : name(std::move(n)), specialty(std::move(spec)),
          hireCost(hire), salaryCostPerTurn(salary) {}

    // Copy constructor
    Advisor(const Advisor& other)
        : name(other.name), specialty(other.specialty),
          hireCost(other.hireCost), salaryCostPerTurn(other.salaryCostPerTurn),
          loyalty(other.loyalty), active(other.active),
          lastRecommendedAction(other.lastRecommendedAction) {
        std::cout << "[Advisor] Copy constructor pentru " << name << '\n';
    }

    // operator=
    Advisor& operator=(const Advisor& other) {
        if (this != &other) {
            name = other.name;
            specialty = other.specialty;
            hireCost = other.hireCost;
            salaryCostPerTurn = other.salaryCostPerTurn;
            loyalty = other.loyalty;
            active = other.active;
            lastRecommendedAction = other.lastRecommendedAction;
        }
        std::cout << "[Advisor] operator= pentru " << name << '\n';
        return *this;
    }

    ~Advisor() {
        std::cout << "[Advisor] Destructor pentru " << name << '\n';
    }

    [[nodiscard]] const std::string& getName() const { return name; }
    [[nodiscard]] bool isActive() const { return active; }
    [[nodiscard]] float getLoyalty() const { return loyalty; }
    [[nodiscard]] int getSalaryCost() const { return salaryCostPerTurn; }
    [[nodiscard]] int getHireCost() const { return hireCost; }

    // Angajeaza advisorul (returneaza costul dedus sau -1 daca nu se poate)
    bool hire(int& playerGold) {
        if (active) {
            std::cout << "[Advisor] " << name << " este deja angajat.\n";
            return false;
        }
        if (playerGold < hireCost) {
            std::cout << "[Advisor] Nu ai destul aur pentru a angaja " << name
                      << " (cost=" << hireCost << ", ai=" << playerGold << ")\n";
            return false;
        }
        playerGold -= hireCost;
        active = true;
        loyalty = 80.0f;
        std::cout << "[Advisor] " << name << " (" << specialty
                  << ") a fost angajat pentru " << hireCost << " aur.\n";
        return true;
    }

    // Plateste salariul; daca playerul nu are bani, loialitatea scade
    void paySalary(int& playerGold) {
        if (!active) return;
        if (playerGold >= salaryCostPerTurn) {
            playerGold -= salaryCostPerTurn;
            std::cout << "[Advisor] Salariu platit lui " << name
                      << " (" << salaryCostPerTurn << " aur).\n";
        } else {
            loyalty -= 15.0f;
            std::cout << "[Advisor] Nu ai putut plati salariul lui " << name
                      << "! Loialitate -15 (acum: " << loyalty << ")\n";
            if (loyalty <= 0.0f) {
                loyalty = 0.0f;
                active = false;
                std::cout << "[Advisor] " << name << " a demisionat!\n";
            }
        }
    }

    // Genereaza o recomandare bazata pe situatia curenta
    // gold si stability sunt ale playerului; returneaza actiunea recomandata (1..5)
    int recommend(int gold, float stability, int ownedCountries) {
        if (!active) return -1;

        int rec = 0;
        if (specialty == "Economic") {
            // Prioritizeaza colectarea de comori si abilitatile economice
            if (gold < 100)
                rec = 3; // deschide chest pentru aur
            else if (stability < 40.0f)
                rec = 4; // tine discurs calm
            else
                rec = 1; // cumpara tara
        } else if (specialty == "Military") {
            // Prioritizeaza cucerirea
            if (stability > 60.0f && gold > 200)
                rec = 2; // cucereste
            else if (stability < 50.0f)
                rec = 4; // stabilizeaza
            else
                rec = 5; // foloseste abilitate militara
        } else { // Diplomatic
            if (ownedCountries == 0)
                rec = 1; // cumpara prima tara
            else if (stability < 55.0f)
                rec = 4; // discurs
            else
                rec = 1; // extinde diplomatic
        }

        lastRecommendedAction = rec;

        // Calitatea sfatului depinde de loialitate
        std::cout << "[Advisor] " << name << " recomanda actiunea " << rec;
        if (loyalty >= 70.0f)
            std::cout << " (sfat de incredere - loialitate ridicata)";
        else if (loyalty >= 40.0f)
            std::cout << " (sfat rezonabil)";
        else
            std::cout << " (sfat incert - loialitate scazuta, ia-l cu rezerve)";
        std::cout << "\n";

        return rec;
    }

    // Playerul anunta daca a urmat sfatul; loialitatea se ajusteaza
    void reportFollowed(bool followed) {
        if (!active || lastRecommendedAction == -1) return;
        if (followed) {
            loyalty += 5.0f;
            if (loyalty > 100.0f) loyalty = 100.0f;
            std::cout << "[Advisor] " << name << " este multumit ca i-ai urmat sfatul. Loialitate +5 ("
                      << loyalty << ")\n";
        } else {
            loyalty -= 8.0f;
            if (loyalty < 0.0f) {
                loyalty = 0.0f;
                active = false;
                std::cout << "[Advisor] " << name << " a demisionat din cauza ca ii ignori constant sfaturile!\n";
            } else {
                std::cout << "[Advisor] " << name << " este dezamagit. Loialitate -8 ("
                          << loyalty << ")\n";
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Advisor& a) {
        os << "Advisor[" << a.name << " | " << a.specialty
           << " | loialitate=" << a.loyalty
           << " | salariu/turn=" << a.salaryCostPerTurn
           << " | " << (a.active ? "ACTIV" : "INACTIV") << "]";
        return os;
    }
};


class Player {
    std::vector<Country*> ownedCountries;
    int goldAmount = 0;
    float stability = 100.0f;
    std::vector<Ability> abilities;

public:
    Player(const std::vector<Country*>& owned,
           int gold,
           float stab,
           const std::vector<Ability>& abil)
        : ownedCountries(owned), goldAmount(gold), stability(stab), abilities(abil) {}

    friend std::ostream& operator<<(std::ostream& os, const Player& p) {
        os << "Player gold=" << p.goldAmount << " stability=" << p.stability
           << " owned=[" ;
        for (size_t i = 0; i < p.ownedCountries.size(); ++i) {
            os << (i? ", " : "") << p.ownedCountries[i]->getName();
        }
        os << "]";
        return os;
    }

    [[nodiscard]] int getGold() const { return goldAmount; }
    [[nodiscard]] float getStability() const { return stability; }
    [[nodiscard]] int getOwnedCount() const { return static_cast<int>(ownedCountries.size()); }

    int& goldRef() { return goldAmount; }

    [[nodiscard]] int calculateTotalIncome() const {
        int total = 0;
        for (const Country* c : ownedCountries) {
            total += c->productionPerTurn();
        }
        if (stability < 50.0f) {
            float factor = 1.0f - (50.0f - stability) / 200.0f;
            total = static_cast<int>(static_cast<float>(total) * factor);
        }
        return total;
    }

    void collectIncome() {
        int inc = calculateTotalIncome();
        goldAmount += inc;
        std::cout << "Ai colectat " << inc << " aur pasiv (total aur acum: " << goldAmount << ")\n";
    }

    bool buyCountry(Country& target) {
        if (target.getOwner() == this) {
            std::cout << "Deja detii " << target.getName() << "\n";
            return false;
        }
        int cost = target.costToBuy();
        if (goldAmount < cost) {
            std::cout << "Nu ai suficiente resurse pentru a cumpara " << target.getName()
                      << " (cost=" << cost << ", ai=" << goldAmount << ")\n";
            return false;
        }

        if (target.getOwner() != nullptr && target.getOwner() != this) {
            cost = static_cast<int>(static_cast<float>(cost) * 1.5f);
            std::cout << "Tara are proprietar, pretul negociat este " << cost << "\n";
            if (goldAmount < cost) {
                std::cout << "Nu ai destui bani pentru a negocia cumpararea.\n";
                return false;
            }
        }

        goldAmount -= cost;
        if (target.getOwner() != nullptr && target.getOwner() != this) {
            stability -= 5.0f;
            std::cout << "Cumparare de la alt proprietar: stabilitatea scade cu 5.\n";
        } else {
            stability -= 1.0f;
        }
        target.setOwner(this);
        ownedCountries.push_back(&target);
        std::cout << "Ai cumparat " << target.getName() << " pentru " << cost << " aur.\n";

        float extraDecay = 0.5f * static_cast<float>(ownedCountries.size() - 1);
        stability -= extraDecay;
        if (stability < 0.0f) stability = 0.0f;
        return true;
    }

    bool conquer(Country& target) {
        if (target.getOwner() == this) {
            std::cout << "Tara este deja a ta.\n";
            return false;
        }

        bool can = false;
        for (const Country* myc : ownedCountries) {
            if (myc->isNeighbor(&target)) {
                can = true;
                break;
            }
        }
        if (!can && !ownedCountries.empty()) {
            std::cout << "Poti cuceri doar tari vecine uneia dintre tarile tale.\n";
            return false;
        }

        int cost = static_cast<int>(static_cast<float>(target.costToBuy()) * 1.2f);
        if (goldAmount < cost) {
            std::cout << "Nu ai destule resurse pentru cucerire (cost=" << cost << ", ai=" << goldAmount << ")\n";
            return false;
        }
        goldAmount -= cost;
        target.setOwner(this);
        ownedCountries.push_back(&target);

        float dec = 2.0f * static_cast<float>(target.getTier());
        stability -= dec;
        if (stability < 0.0f) stability = 0.0f;
        std::cout << "Ai cucerit " << target.getName() << " pentru " << cost << " aur. Stabilitate -" << dec << "\n";
        return true;
    }

    void giveSpeech(int choice) {
        std::cout << "Tii un discurs (alegere: " << choice << ")\n";
        switch (choice) {
            case 1:
                stability -= 5.0f;
                std::cout << "Discurs agresiv: stabilitatea scade cu 5, dar vei primi +10% income urmatorul turn.\n";
                break;
            case 2:
                stability += 7.0f;
                if (stability > 100.0f) stability = 100.0f;
                std::cout << "Discurs calm: stabilitatea creste cu 7.\n";
                break;
            case 3:
                stability += 2.0f;
                if (stability > 100.0f) stability = 100.0f;
                std::cout << "Discurs echilibrat: stabilitatea creste cu 2.\n";
                break;
            default:
                std::cout << "Alegere invalida. Nimic nu se intampla.\n";
        }
    }

    bool useAbility(size_t idx) {
        if (idx >= abilities.size()) {
            std::cout << "Ability index invalid.\n";
            return false;
        }
        const Ability &ab = abilities[idx];
        if (goldAmount < ab.getGoldCost()) {
            std::cout << "Nu ai aur pentru a folosi ability-ul " << ab.getName() << "\n";
            return false;
        }
        goldAmount -= ab.getGoldCost();
        stability += static_cast<float>(ab.getStabilityEffect());
        if (stability > 100.0f) stability = 100.0f;
        std::cout << "Ai folosit ability-ul " << ab.getName() << ". Stabilitate modificata cu "
                  << ab.getStabilityEffect() << "\n";
        if (ab.isConsumable()) {
            abilities.erase(abilities.begin() + idx);
        }
        return true;
    }

    void listAbilities() const {
        std::cout << "Abilities (" << abilities.size() << "):\n";
        for (size_t i = 0; i < abilities.size(); ++i) {
            std::cout << "  [" << i << "] " << abilities[i] << "\n";
        }
    }
};


int main() {

    Country rom("Romania", {}, nullptr, 3, 2);
    Country bul("Bulgaria", {}, nullptr, 2, 1);
    Country hun("Ungaria", {}, nullptr, 4, 3);
    Country gre("Grecia", {}, nullptr, 1, 1);

    rom = Country("Romania", {&bul, &hun}, nullptr, 3, 2);
    bul = Country("Bulgaria", {&rom, &gre}, nullptr, 2, 1);
    hun = Country("Hungary", {&rom}, nullptr, 4, 3);
    gre = Country("Greece", {&bul}, nullptr, 1, 1);

    std::vector<Ability> abil = {
        Ability("PeoplePleaser", "Scade pierderea stabilitatii temporar", 80, +8, true),
        Ability("MilitaryPower", "Reduce costurile de cumparare (simulat ca bonus stabilitate)", 60, +3, true)
    };

    Player player({}, 500, 90.0f, abil);

    // Creare advisori
    // Unul economic (sfatuieste spre aur), unul militar (spre cucerire)
    Advisor econAdvisor("Mihai Viteazul", "Economic", 100, 20);
    Advisor milAdvisor("Stefan cel Mare", "Military", 150, 30);

    std::cout << "Start game:\n" << player << "\n";
    std::cout << "--- Advisori disponibili:\n";
    std::cout << econAdvisor << "\n" << milAdvisor << "\n";
    std::cout << "--- Harta (tari):\n";
    std::cout << rom << "\n" << bul << "\n" << hun << "\n" << gre << "\n";

    // Angajeaza automat advisorul economic la start
    std::cout << "\n--- Angajare advisor economic la start ---\n";
    econAdvisor.hire(player.goldRef());
    std::cout << econAdvisor << "\n";

    for (int turn = 1; turn <= 3; ++turn) {
        std::cout << "\n====== Turn " << turn << " ======\n";

        player.collectIncome();

        // Plateste salariile advisorilor activi
        econAdvisor.paySalary(player.goldRef());
        milAdvisor.paySalary(player.goldRef());

        // Advisorul activ ofera o recomandare
        int advisorRec = -1;
        if (econAdvisor.isActive()) {
            advisorRec = econAdvisor.recommend(
                player.getGold(), player.getStability(), player.getOwnedCount());
        } else if (milAdvisor.isActive()) {
            advisorRec = milAdvisor.recommend(
                player.getGold(), player.getStability(), player.getOwnedCount());
        }

        std::cout << "Actiuni disponibile:\n";
        std::cout << "  1) Cumpara o tara\n";
        std::cout << "  2) Cucereste o tara (daca e vecina)\n";
        std::cout << "  3) Deschide un ResourceChest (quiz)\n";
        std::cout << "  4) Tine un discurs (alegere 1..3)\n";
        std::cout << "  5) Foloseste o abilitate\n";
        std::cout << "  6) Angajeaza advisorul militar (daca nu e activ)\n";
        std::cout << "  0) Skip\n";
        std::cout << "Alege actiunea (numar): ";

        int action = -1;
        if (!(std::cin >> action)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            action = 0;
        }

        // Raporteaza la advisor daca playerul i-a urmat sau nu sfatul
        if (advisorRec != -1) {
            bool followed = (action == advisorRec);
            if (econAdvisor.isActive())
                econAdvisor.reportFollowed(followed);
            else if (milAdvisor.isActive())
                milAdvisor.reportFollowed(followed);
        }

        if (action == 1) {
            std::cout << "Ce tara vrei sa cumperi? (1 Romania, 2 Bulgaria, 3 Ungaria, 4 Grecia): ";
            int c = 0; if(!(std::cin >> c)) { std::cin.clear(); std::cin.ignore(); c = 0; }
            Country* target = nullptr;
            if (c == 1) target = &rom;
            else if (c == 2) target = &bul;
            else if (c == 3) target = &hun;
            else if (c == 4) target = &gre;
            if (target) player.buyCountry(*target);
        } else if (action == 2) {
            std::cout << "Ce tara vrei sa cuceresti? (1 Romania, 2 Bulgaria, 3 Hungary, 4 Greece): ";
            int c = 0; if(!(std::cin >> c)) { std::cin.clear(); std::cin.ignore(); c = 0; }
            Country* target = nullptr;
            if (c == 1) target = &rom;
            else if (c == 2) target = &bul;
            else if (c == 3) target = &hun;
            else if (c == 4) target = &gre;
            if (target) player.conquer(*target);
        } else if (action == 3) {
            ResourceChest rc(
                "Care este capitala Romaniei?",
                {"Sofia", "Bucuresti", "Budapesta"},
                1,
                150,
                3
            );
            int reward = rc.presentAndResolve();
            if (reward > 0) {
                std::cout << "Se adauga reward la aur.\n";
            }
        } else if (action == 4) {
            std::cout << "Alege tip discurs: 1=bold,2=calm,3=medium : ";
            int ch = 0; if(!(std::cin >> ch)) { std::cin.clear(); std::cin.ignore(); ch = 0; }
            player.giveSpeech(ch);
        } else if (action == 5) {
            player.listAbilities();
            std::cout << "Alege index abilitate: ";
            size_t idx = 0;
            if(!(std::cin >> idx)) {
                std::cin.clear(); std::cin.ignore();
                idx = std::numeric_limits<size_t>::max();
            }
            if (idx != std::numeric_limits<size_t>::max()) player.useAbility(idx);
        } else if (action == 6) {
            if (!milAdvisor.isActive()) {
                milAdvisor.hire(player.goldRef());
            } else {
                std::cout << "Advisorul militar este deja activ.\n";
            }
        } else {
            std::cout << "Skip action.\n";
        }

        // Afisare stare advisori la sfarsitul turnului
        std::cout << "--- Stare advisori:\n";
        std::cout << "  " << econAdvisor << "\n";
        std::cout << "  " << milAdvisor  << "\n";

        std::cout << "Sfarsit turn " << turn << ". " << player << "\n";
    }

    std::cout << "\nFinal game state:\n" << player << "\n";
    std::cout << "Harta finala:\n" << rom << "\n" << bul << "\n" << hun << "\n" << gre << "\n";

    return 0;
}