package com.Starygin.Presentation.Scenarios;

import com.Starygin.Application.Contracts.UserService;
import com.Starygin.Presentation.Scenario;

import java.io.IOException;
import java.util.Scanner;

public class LoginScenario implements Scenario {
    private final UserService userService;

    public LoginScenario(UserService userService) {
        this.userService = userService;
    }

    @Override
    public String getName() {
        return "Login Scenario";
    }

    @Override
    public void run() {
        Scanner s = new Scanner(System.in);
        System.out.print("Enter your username: ");

        String userName = s.nextLine();

        System.out.print("Enter your password: ");

        String password = s.nextLine();

        try {
            userService.login(userName, password);
            System.out.println("Successfully logged in!");

        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }
}
