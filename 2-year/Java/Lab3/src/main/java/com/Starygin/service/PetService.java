package com.Starygin.service;

import com.Starygin.model.repository.PetRepository;
import com.Starygin.model.dto.PetDto;
import com.Starygin.service.exceptions.PetIdMismatchException;
import com.Starygin.service.exceptions.PetNotFoundException;
import com.Starygin.service.mapper.PetMapper;
import com.querydsl.core.types.Predicate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Pageable;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Objects;

@Service
public class PetService {
    private final PetRepository petRepository;

    private final PetMapper petMapper;

    @Autowired
    public PetService(PetRepository petRepository, PetMapper petMapper) {
        this.petRepository = petRepository;
        this.petMapper = petMapper;
    }

    public List<PetDto> findAll() {
        return petRepository.findAll().stream().map(petMapper::toDto).toList();
    }

    public List<PetDto> findAllByPageAndFilter(Predicate predicate,
                                               Pageable pageable) {

        return petRepository.findAll(predicate, pageable)
                .stream()
                .map(petMapper::toDto)
                .toList();
    }

    public List<PetDto> findByName(String petName) {
        return petRepository.findByName(petName).stream().map(petMapper::toDto).toList();
    }


    public PetDto findOne(Long id) {
        return petMapper.toDto(petRepository.findById(id).orElseThrow(PetNotFoundException::new));
    }

    public PetDto create(PetDto petDto) {
        return petMapper.toDto(petRepository.save(petMapper.toEntity(petDto)));
    }

    public void delete(Long id) {
        petRepository.findById(id).orElseThrow(PetNotFoundException::new);
        petRepository.deleteById(id);
    }

    public PetDto updatePet(PetDto petDto, Long id) {
        if (!Objects.equals(petDto.id(), id)) {
            throw new PetIdMismatchException();
        }

        petRepository.findById(id).orElseThrow(PetNotFoundException::new);

        return petMapper.toDto(petRepository.save(petMapper.toEntity(petDto)));
    }
}
