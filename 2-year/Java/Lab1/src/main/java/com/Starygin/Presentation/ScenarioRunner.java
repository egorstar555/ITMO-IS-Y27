package com.Starygin.Presentation;


import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class ScenarioRunner {
    private final List<ScenarioProvider> providers;

    public ScenarioRunner(List<ScenarioProvider> providers) {
        this.providers = providers;
    }

    public void run() {
        List<Scenario> scenarios = new ArrayList<>(getScenarios());

        var selectedScenario = selectScenario(scenarios);

        if (selectedScenario != null) {
            selectedScenario.run();
        }
    }

    private Scenario selectScenario(List<Scenario> scenarios) {
        for (int i = 0; i < scenarios.size(); i++) {
            System.out.println(i + ": " + scenarios.get(i).getName());
        }

        System.out.println("Select scenario");

        Scanner s = new Scanner(System.in);

        int choice = s.nextInt();

        return scenarios.get(choice);
    }

    private List<Scenario> getScenarios() {
        List<Scenario> scenarios = new ArrayList<>();
        for (ScenarioProvider provider : providers) {
            var scenario = provider.tryGetScenario();
            if (scenario != null) {
                scenarios.add(scenario);
            }
        }

        return scenarios;
    }
}
