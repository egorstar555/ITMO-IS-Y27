package com.Starygin.Presentation.Scenarios;

import com.Starygin.Application.Contracts.UserService;
import com.Starygin.Presentation.Scenario;

import java.io.IOException;

public class CreateAccountScenario implements Scenario {
    private final UserService userService;

    public CreateAccountScenario(UserService userService) {
        this.userService = userService;
    }

    @Override
    public String getName() {
        return "Create Account";
    }

    @Override
    public void run() {
        try {
            long accountId = userService.createAccount();

            System.out.println("Successfully created account, id: " + accountId);

        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }
}
