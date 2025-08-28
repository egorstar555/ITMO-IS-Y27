package com.Starygin.model.entity;

import org.springframework.security.core.GrantedAuthority;

public enum USER_ROLE implements GrantedAuthority {
    admin,
    user,
    ;

    @Override
    public String getAuthority() {
        return this.name();
    }
}
