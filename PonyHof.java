package uebungen;

import java.util.*;

class PonyhofSimulation {
    // Eintrittspreise
    private static final Map<String, Integer> EINTRITTSPREISE = Map.of(
            "Erwachsene", 15,
            "Kinder (bis 7 Jahre)", 5,
            "Kinder (8 bis 13 Jahre)", 7,
            "Rentner", 10,
            "Studenten", 9
    );

    // Attraktionen
    private static final Map<String, Integer> ATTRAKTIONEN = Map.of(
            "Planwagenfahrt", 7,
            "Ponyreiten", 5,
            "Hühner füttern", 2,
            "Brot backen", 5,
            "Streichelzoo", 3
    );

    // Essen und Trinken
    private static final Map<String, Integer> ESSEN_TRINKEN = Map.of(
            "Wasser", 2,
            "Limo", 3,
            "Waffeln", 3,
            "Bratwurst", 2,
            "Salat", 6,
            "Currywurst", 4,
            "Pommes", 2,
            "Veganer Wrap", 5
    );

    private static final Random random = new Random(1111);

    public static void main(String[] args) {
        // Besucherzahlen
        int besucherAnzahl = random.nextInt(101) + 150;
        Map<String, Integer> besucherKategorien = new HashMap<>();
        int gesamteinnahmen_eintritt = 0;

        for (String kategorie : EINTRITTSPREISE.keySet()) {
            besucherKategorien.put(kategorie, 0);
        }

        // Besucher generieren
        List<String> besucherListe = new ArrayList<>();
        for (int i = 0; i < besucherAnzahl; i++) {
            String kategorie = getRandomCategory();
            besucherListe.add(kategorie);
            besucherKategorien.put(kategorie, besucherKategorien.get(kategorie) + 1);
            gesamteinnahmen_eintritt += EINTRITTSPREISE.get(kategorie);

            // Kinder unter 7 mit Begleitung
            if (kategorie.equals("Kinder (bis 7 Jahre)")) {
                String begleitung = random.nextBoolean() ? "Erwachsene" : "Rentner";
                besucherListe.add(begleitung);
                besucherKategorien.put(begleitung, besucherKategorien.get(begleitung) + 1);
                gesamteinnahmen_eintritt += EINTRITTSPREISE.get(begleitung);
            }
        }

        // Attraktionen & Verpflegung
        Map<String, Integer> attraktionNutzung = new HashMap<>();
        Map<String, Integer> essenTrinkenBestellung = new HashMap<>();
        int attraktionEinnahmen = 0, verpflegungEinnahmen = 0;

        for (String attraktion : ATTRAKTIONEN.keySet()) attraktionNutzung.put(attraktion, 0);
        for (String item : ESSEN_TRINKEN.keySet()) essenTrinkenBestellung.put(item, 0);

        for (String besucher : besucherListe) {
            List<String> attraktionen = getRandomEntries(ATTRAKTIONEN.keySet(), 3);
            List<String> essen = getRandomEntries(ESSEN_TRINKEN.keySet(), random.nextInt(3) + 2);

            for (String attraktion : attraktionen) {
                attraktionNutzung.put(attraktion, attraktionNutzung.get(attraktion) + 1);
                attraktionEinnahmen += ATTRAKTIONEN.get(attraktion);
            }
            for (String item : essen) {
                essenTrinkenBestellung.put(item, essenTrinkenBestellung.get(item) + 1);
                verpflegungEinnahmen += ESSEN_TRINKEN.get(item);
            }
        }

        // Gesamteinnahmen berechnen
        int gesamteinnahmen = gesamteinnahmen_eintritt + attraktionEinnahmen + verpflegungEinnahmen;

        // Ausgabe
        System.out.println("Gesamtbesucher: " + besucherListe.size());
        besucherKategorien.forEach((k, v) -> System.out.println(k + ": " + v));
        System.out.println("Einnahmen aus Eintritt: " + gesamteinnahmen_eintritt);
        System.out.println("Attraktionen Nutzung:");
        attraktionNutzung.forEach((k, v) -> System.out.println(k + ": " + v));
        System.out.println("Einnahmen Attraktionen: " + attraktionEinnahmen);
        System.out.println("Speisen/Trinken:");
        essenTrinkenBestellung.forEach((k, v) -> System.out.println(k + ": " + v));
        System.out.println("Einnahmen Verpflegung: " + verpflegungEinnahmen);
        System.out.println("Gesamteinnahmen: " + gesamteinnahmen);
    }

    private static String getRandomCategory() {
        List<String> keys = new ArrayList<>(EINTRITTSPREISE.keySet());
        return keys.get(random.nextInt(keys.size()));
    }

    private static List<String> getRandomEntries(Set<String> set, int count) {
        List<String> list = new ArrayList<>(set);
        Collections.shuffle(list);
        return list.subList(0, count);
    }
}
