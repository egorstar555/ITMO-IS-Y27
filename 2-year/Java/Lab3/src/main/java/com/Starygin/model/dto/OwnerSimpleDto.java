package com.Starygin.model.dto;

import java.time.LocalDate;

public record OwnerSimpleDto(Long id, String name, LocalDate birthday) {}