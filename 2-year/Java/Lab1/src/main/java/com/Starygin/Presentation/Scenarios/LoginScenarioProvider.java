package com.Starygin.Presentation.Scenarios;

import com.Starygin.Application.Contracts.CurrentUserService;
import com.Starygin.Application.Contracts.UserService;
import com.Starygin.Presentation.Scenario;
import com.Starygin.Presentation.ScenarioProvider;

public class LoginScenarioProvider implements ScenarioProvider {
    private final UserService userService;

    private final CurrentUserService currentUserService;

    public LoginScenarioProvider(UserService userService, CurrentUserService currentUserService) {
        this.userService = userService;
        this.currentUserService = currentUserService;
    }

    @Override
    public Scenario tryGetScenario() {
        if (currentUserService.getUser().isPresent()) {
            return null;
        }

        return new LoginScenario(userService);
    }
}
